#include "Window.h"
#include "Engine/Engine.h"
#include "Engine/Event/EventManager.h"
#include "Engine/ECS/SystemManager.h"
#include "Engine/Input/InputSystem.h"

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

		mMessageCallback = [this](HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)->LRESULT 
		{
			if (mHandle != hWnd)
			{
				return 0;
			}

			EventManager* eventManager = Engine::GetInstance()->GetEventManager();

			switch (Msg)
			{
				// NOTE: Window events
				case WM_CLOSE:
				{
					eventManager->Post<WindowCloseEvent>(mHandle);
					return 0;
				}
				case WM_SETFOCUS:
				{
					eventManager->Post<WindowSetFocusEvent>(mHandle);
					break;
				}
				case WM_KILLFOCUS:
				{
					eventManager->Post<WindowKillFocusEvent>(mHandle);
					break;
				}
				case WM_SIZE:
				{
					if (wParam == SIZE_MINIMIZED)
					{
						eventManager->Post<WindowMinimizedEvent>(mHandle);
						break;
					}
					else if (wParam == SIZE_MAXIMIZED)
					{
						eventManager->Post<WindowMaximizedEvent>(mHandle);
						break;
					}
					else
					{
						mWidth = (UINT)LOWORD(lParam);
						mHeight = (UINT)HIWORD(lParam);
						eventManager->Post<WindowSizeEvent>(mHandle, mWidth, mHeight);
						break;
					}
				}
				case WM_SETCURSOR:
				{
					HCURSOR cursor = ::LoadCursor(nullptr, IDC_ARROW);
					::SetCursor(cursor);
					break;
				}
				// NOTE: Keyboard events
				case WM_KEYUP:
					[[fallthrough]];
				case WM_SYSKEYUP:
				{
					break;
				}
				case WM_KEYDOWN:
					[[fallthrough]];
				case WM_SYSKEYDOWN:
				{
					break;
				}
				case WM_MOUSEMOVE:
				{
					const POINTS points = MAKEPOINTS(lParam);
					
					const auto& mouse = Engine::GetInstance()->GetSystemManager()->GetSystem<InputSystem>()->GetMouse();

					if (!mIsCursorEnabled)
					{
						if (!mouse.IsInWindow())
						{
							::SetCapture(mHandle);
							eventManager->Post<MouseEnterEvent>(mHandle);
							while (::ShowCursor(FALSE) >= 0);
						}
						break;
					}

					const auto& x = points.x;
					const auto& y = points.y;

					if (x >= 0 && x < static_cast<int>(mWidth) && y >= 0 && y < static_cast<int>(mHeight))
					{
						eventManager->Post<MouseMoveEvent>(x, y);

						if (!mouse.IsInWindow())
						{
							::SetCapture(mHandle);
							eventManager->Post<MouseEnterEvent>(mHandle);
						}
					}
					else
					{
						if (wParam & (MK_LBUTTON | MK_RBUTTON))
						{
							eventManager->Post<MouseMoveEvent>(x, y);
						}
						else
						{
							::ReleaseCapture();
							eventManager->Post<MouseLeaveEvent>(mHandle);
						}
					}
					break;
				}
				case WM_LBUTTONDOWN:
				{
					break;
				}
				case WM_RBUTTONDOWN:
				{
					break;
				}
				case WM_LBUTTONUP:
				{
					break;
				}
				case WM_RBUTTONUP:
				{
					break;
				}
				case WM_MOUSEWHEEL:
				{
					break;
				}
				case WM_INPUT:
				{
					std::vector<uint8_t> buffer;

					UINT size = 0;
					::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
					buffer.resize(size);
					::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));

					const auto& rawInput = reinterpret_cast<const RAWINPUT&>(*buffer.data());

					if (rawInput.header.dwType == RIM_TYPEMOUSE && (rawInput.data.mouse.lLastX != 0 || rawInput.data.mouse.lLastY != 0))
					{
						eventManager->Post<MouseRawInputEvent>(rawInput.data.mouse.lLastX, rawInput.data.mouse.lLastY);
					}

					break;
				}
			}

			return ::DefWindowProc(hWnd, Msg, wParam, lParam);
		};

		// NOTE: we wanna to immediately notify that window is created
		Engine::GetInstance()->GetEventManager()->Invoke<WindowCreateEvent>(mHandle, mWidth, mHeight);

		::ShowWindow(mHandle, SW_SHOWDEFAULT);
		::UpdateWindow(mHandle);

		return true;
	}

	void Window::Terminate()
	{
		// NOTE: we wanna to immediately notify that window is terminated
		Engine::GetInstance()->GetEventManager()->Invoke<WindowDestroyEvent>(mHandle);

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

	void Window::SetCursorEnable(const bool IsEnabled)
	{
		mIsCursorEnabled = IsEnabled;

		if (mIsCursorEnabled)
		{
			while (::ShowCursor(TRUE) < 0);
			::ClipCursor(nullptr);
		}
		else
		{
			while (::ShowCursor(FALSE) >= 0);
			RECT rectangle;
			::GetClientRect(mHandle, &rectangle);
			::MapWindowPoints(mHandle, nullptr, reinterpret_cast<LPPOINT>(&rectangle), 2);
			::ClipCursor(&rectangle);
		}
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