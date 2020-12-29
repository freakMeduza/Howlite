#include "Window.h"

namespace Howlite {

	size_t Window::WINDOW_INSTANCE_COUNT = 0;

	bool Window::Init(const uint32_t InWidth, const uint32_t InHeight)
	{
		const std::string title = "Howlite Window";
		return Init(InWidth, InHeight, title);
	}

	bool Window::Init(const uint32_t InWidth, const uint32_t InHeight, const std::string& InTitle)
	{
		return Init(InWidth, InHeight, InTitle, Style::Close | Style::Resize | Style::Title);
	}

	bool Window::Init(const uint32_t InWidth, const uint32_t InHeight, const std::string& InTitle, const Style InStyle)
	{
		if (WINDOW_INSTANCE_COUNT == 0)
		{
			RegisterWindowClass();
		}

		// Compute position and size
		HDC screenDC = ::GetDC(nullptr);
		int left = (::GetDeviceCaps(screenDC, HORZRES) - static_cast<int>(InWidth)) / 2;
		int top = (::GetDeviceCaps(screenDC, VERTRES) - static_cast<int>(InHeight)) / 2;
		int width = InWidth;
		int height = InHeight;
		::ReleaseDC(nullptr, screenDC);

		DWORD style = WS_VISIBLE;

		if ((uint8_t)(InStyle == Style::Empty))
		{
			style |= WS_POPUP;
		}
		else
		{
			if ((uint8_t)(InStyle & Style::Title))
			{
				style |= WS_CAPTION | WS_MINIMIZEBOX;
			}

			if ((uint8_t)(InStyle & Style::Resize))
			{
				style |= WS_THICKFRAME | WS_MINIMIZEBOX;
			}

			if ((uint8_t)(InStyle & Style::Close))
			{
				style |= WS_SYSMENU;
			}
		}

		mIsFullScreen = (uint8_t)(InStyle & Style::Fullscreen);

		if (!mIsFullScreen)
		{
			RECT rectangle = { 0, 0, width, height };
			::AdjustWindowRect(&rectangle, style, false);
			width = rectangle.right - rectangle.left;
			height = rectangle.bottom - rectangle.top;
		}

		mHandle = ::CreateWindow(WINDOW_CLASS_NAME, InTitle.c_str(), style, left,
			top, width, height, nullptr, nullptr, ::GetModuleHandle(nullptr), this);

		if (!mHandle)
		{
			char* buffer = nullptr;

			const DWORD length = FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				::GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPSTR>(&buffer),
				0,
				nullptr
			);

			std::string errorMessage = buffer;

			if (length == 0)
			{
				errorMessage = "Unknown error code";
			}

			LocalFree(buffer);

			HL_ASSERT(false, errorMessage.c_str());
			return false;
		}

		if (mIsFullScreen)
		{
			SetupFullScreenMode(InWidth, InHeight);
		}

		RECT rectangle;
		::GetClientRect(mHandle, &rectangle);
		mWidth = rectangle.right - rectangle.left;
		mHeight = rectangle.bottom - rectangle.top;

		WINDOW_INSTANCE_COUNT++;

		::ShowWindow(mHandle, SW_SHOWDEFAULT);
		::UpdateWindow(mHandle);

		return true;
	}

	void Window::Terminate()
	{
		if (mIsFullScreen)
		{
			// Restore the previous video mode (in case we were running in fullscreen)
			::ChangeDisplaySettings(nullptr, 0);
		}

		::DestroyWindow(mHandle);
		mHandle = nullptr;
		
		WINDOW_INSTANCE_COUNT--;

		if (WINDOW_INSTANCE_COUNT == 0)
		{
			UnregisterWindowClass();
		}
	}

	void Window::SetMessageCallback(MessageCallback&& InMessageCallback)
	{
		mMessageCallback = std::move(InMessageCallback);
	}

	void Window::RegisterWindowClass()
	{
		WNDCLASS windowClass;
		ZeroMemory(&windowClass, sizeof(windowClass));
		windowClass.style = 0;
		windowClass.lpfnWndProc = &Window::WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = ::GetModuleHandle(nullptr);
		windowClass.hIcon = nullptr;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = WINDOW_CLASS_NAME;
		::RegisterClass(&windowClass);
	}

	void Window::UnregisterWindowClass()
	{
		::UnregisterClass(WINDOW_CLASS_NAME, ::GetModuleHandle(nullptr));
	}

	void Window::SetupFullScreenMode(const uint32_t InWidth, const uint32_t InHeight)
	{
		DEVMODE mode;
		ZeroMemory(&mode, sizeof(mode));
		mode.dmSize = sizeof(mode);
		mode.dmPelsWidth = InWidth;
		mode.dmPelsHeight = InHeight;
		mode.dmBitsPerPel = 32u; // default 32-bits per pixel
		mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Apply fullscreen mode
		if (::ChangeDisplaySettings(&mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			HL_ERROR("Failed to change display settings to full screen mode");
		}

		// Make the window flags compatible with fullscreen mode
		::SetWindowLongW(mHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		::SetWindowLongW(mHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);
		// Resize the window so that it fits the entire screen
		::SetWindowPos(mHandle, HWND_TOP, 0, 0, InWidth, InHeight, SWP_FRAMECHANGED);
	}

	LRESULT Window::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (Msg == WM_CREATE)
		{
			Window* window = reinterpret_cast<Window*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		}

		Window* window = hWnd ? reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)) : nullptr;

		if (window && window->mMessageCallback)
		{
			return window->mMessageCallback(hWnd, Msg, wParam, lParam);
		}

		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}