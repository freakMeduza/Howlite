#include "HowlitePCH.h"
#include "Window.h"
#include "Engine.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Graphic/GraphicSystem.h"
#include "Graphic/UI/UISystem.h"
#include "Input/InputSystem.h"

namespace Howlite {

	HWindow::HWindow(HINSTANCE Instance, const char* Title, uint32_t Width, uint32_t Height) :
		mInstance{ Instance }, mWidth{ Width }, mHeight{ Height }
	{
		RegisterWindowClass();

		const DWORD style = WS_OVERLAPPEDWINDOW;

		RECT rect{ 0, 0, (LONG)Width, (LONG)Height };

		if(::AdjustWindowRect(&rect, style, FALSE) == FALSE)
		{
			H_ERROR(::GetLastError());
		}

		const auto width = rect.right - rect.left;
		const auto height = rect.bottom - rect.top;

		mHandler = ::CreateWindow(mWindowClassName,
								  Title,
								  style,
								  CW_USEDEFAULT,
								  CW_USEDEFAULT,
								  width,
								  height,
								  nullptr,
								  nullptr,
								  mInstance,
								  this);

		if(mHandler == nullptr)
		{
			H_ERROR(::GetLastError());
		}

		RAWINPUTDEVICE device;
		device.usUsagePage = 0x01; // mouse page
		device.usUsage     = 0x02; // mouse usage
		device.dwFlags     = 0;
		device.hwndTarget  = nullptr;

		if (RegisterRawInputDevices(&device, 1, sizeof(device)) == FALSE)
		{
			H_ERROR(GetLastError())
		}

		::ShowWindow(mHandler, SW_SHOWDEFAULT);
		::UpdateWindow(mHandler);
	}

	HWindow::~HWindow()
	{
		::DestroyWindow(mHandler);
		UnregisterWindowClass();
	}

	void HWindow::ProcessMessages()
	{
		MSG msg;

		ZeroMemory(&msg, sizeof(msg));

		while (::PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	void HWindow::SetMessageCallback(std::function<void(class IHEvent&)> MesageCallback) noexcept
	{
		mMessageCallback = MesageCallback;
	}

	HWND HWindow::GetWindowHandler() const noexcept
	{
		return mHandler;
	}

	uint32_t HWindow::GetWidth() const noexcept
	{
		return mWidth;
	}

	uint32_t HWindow::GetHeight() const noexcept
	{
		return mHeight;
	}

	void HWindow::RegisterWindowClass()
	{
		WNDCLASSEX windowClass;

		ZeroMemory(&windowClass, sizeof(windowClass));

		windowClass.cbSize        = sizeof(windowClass);
		windowClass.hIcon         = static_cast<HICON>(LoadImage(mInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0u));
		windowClass.hIconSm       = static_cast<HICON>(LoadImage(mInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0u));
		windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		windowClass.hInstance     = mInstance;
		windowClass.cbClsExtra    = 0;
		windowClass.cbWndExtra    = 0;
		windowClass.style         = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc   = &HWindow::HandleMessage;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName  = nullptr;
		windowClass.lpszClassName = mWindowClassName;

		::RegisterClassEx(&windowClass);
	}

	void HWindow::UnregisterWindowClass()
	{
		::UnregisterClass(mWindowClassName, mInstance);
	}

	LRESULT HWindow::HandleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if(HUISystem::HandleMessage(hWnd, Msg, wParam, lParam))
		{
			return true;
		}

		switch (Msg)
		{
			/**
			 * Window Event
			 */
			case WM_CREATE:
			{
				HWindow* window = reinterpret_cast<HWindow*>(reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams);
				::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
				break;
			}
			case WM_CLOSE:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if (window->mMessageCallback)
				{
					HWindowClosedEvent event;
					window->mMessageCallback(event);
				}
				return 0;
			}
			case WM_SIZE:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				window->mWidth = (UINT)LOWORD(lParam);
				window->mHeight = (UINT)HIWORD(lParam);
				if (window->mMessageCallback && wParam != SIZE_MINIMIZED)
				{
					HWindowResizedEvent event{ window->mWidth, window->mHeight };
					window->mMessageCallback(event);
				}
				break;
			}

			/**
			 * Keyboard Event
			 */
			case WM_KEYDOWN:
				[[fallthrough]];
			case WM_SYSKEYDOWN:
			{
				if (!(lParam & 0x40000000))
				{
					HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
					if(window->mMessageCallback)
					{
						HKeyPressedEvent event{ static_cast<unsigned char>(wParam) };
						window->mMessageCallback(event);
					}
				}

				break;
			}
			case WM_KEYUP:
				[[fallthrough]];
			case WM_SYSKEYUP:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HKeyReleasedEvent event{ static_cast<unsigned char>(wParam) };
					window->mMessageCallback(event);
				}
				break;
			}

			/**
			 * Mouse Messages
			 */
			case WM_MOUSEMOVE:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				POINTS p = MAKEPOINTS(lParam);
				if (p.x >= 0 && p.x < static_cast<int>(window->mWidth) && p.y >= 0 && p.y < static_cast<int>(window->mHeight))
				{
					if(window->mMessageCallback)
					{
						HMouseMovedEvent movedEvent{ { p.x, p.y } };
						window->mMessageCallback(movedEvent);
					}
					if (!HEngine::GetInstance().GetInputSystemInstance().IsMouseInWindow())
					{
						::SetCapture(window->GetWindowHandler());
						HEngine::GetInstance().GetInputSystemInstance().SetMouseInWindow(true);
						if(window->mMessageCallback)
						{
							HMouseEnteredEvent enteredEvent;
							window->mMessageCallback(enteredEvent);
						}
					}
				}
				else
				{
					if (wParam & (MK_LBUTTON | MK_RBUTTON))
					{
						if(window->mMessageCallback)
						{
							HMouseMovedEvent movedEvent{ { p.x, p.y } };
							window->mMessageCallback(movedEvent);
						}
					}
					else
					{
						::ReleaseCapture();
						HEngine::GetInstance().GetInputSystemInstance().SetMouseInWindow(false);
						if(window->mMessageCallback)
						{
							HMouseLeavedEvent leavedEvent;
							window->mMessageCallback(leavedEvent);
						}
					}
				}

				break;
			}
			case WM_LBUTTONDOWN:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonPressedEvent event{ VK_LBUTTON };
					window->mMessageCallback(event);
				}
				break;
			}
			case WM_RBUTTONDOWN:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonPressedEvent event{ VK_RBUTTON };
					window->mMessageCallback(event);
				}
				break;
			}
			case WM_MBUTTONDOWN:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonPressedEvent event{ VK_MBUTTON };
					window->mMessageCallback(event);
				}
				break;
			}
			case WM_LBUTTONUP:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonReleasedEvent releasedEvent{ VK_LBUTTON };
					window->mMessageCallback(releasedEvent);
				}
				
				const POINTS p = MAKEPOINTS(lParam);

				if (p.x < 0 || p.x >= static_cast<int>(window->mWidth) || p.y < 0 || p.y >= static_cast<int>(window->mHeight))
				{
					::ReleaseCapture();
					HEngine::GetInstance().GetInputSystemInstance().SetMouseInWindow(false);
					if(window->mMessageCallback)
					{
						HMouseLeavedEvent leavedEvent;
						window->mMessageCallback(leavedEvent);
					}
				}

				break;
			}
			case WM_RBUTTONUP:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonReleasedEvent releasedEvent{ VK_RBUTTON };
					window->mMessageCallback(releasedEvent);
				}
				
				const POINTS p = MAKEPOINTS(lParam);

				if (p.x < 0 || p.x >= static_cast<int>(window->mWidth) || p.y < 0 || p.y >= static_cast<int>(window->mHeight))
				{
					::ReleaseCapture();
					HEngine::GetInstance().GetInputSystemInstance().SetMouseInWindow(false);
					if(window->mMessageCallback)
					{
						HMouseLeavedEvent leavedEvent;
						window->mMessageCallback(leavedEvent);
					}
				}

				break;
			}
			case WM_MBUTTONUP:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				if(window->mMessageCallback)
				{
					HMouseButtonReleasedEvent releasedEvent{ VK_MBUTTON };
					window->mMessageCallback(releasedEvent);
				}

				const POINTS p = MAKEPOINTS(lParam);

				if (p.x < 0 || p.x >= static_cast<int>(window->mWidth) || p.y < 0 || p.y >= static_cast<int>(window->mHeight))
				{
					::ReleaseCapture();
					HEngine::GetInstance().GetInputSystemInstance().SetMouseInWindow(false);
					if(window->mMessageCallback)
					{
						HMouseLeavedEvent leavedEvent;
						window->mMessageCallback(leavedEvent);
					}
				}

				break;
			}
			case WM_MOUSEWHEEL:
			{
				static int scrollDelta = 0;
				const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				scrollDelta += delta;
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

				while (scrollDelta >= WHEEL_DELTA)
				{
					scrollDelta -= WHEEL_DELTA;
					if(window->mMessageCallback)
					{
						HMouseScrolledUpEvent event;
						window->mMessageCallback(event);
					}
				}

				while (scrollDelta <= -WHEEL_DELTA)
				{
					scrollDelta += WHEEL_DELTA;
					if(window->mMessageCallback)
					{
						HMouseScrolledDownEvent event;
						window->mMessageCallback(event);
					}
				}

				break;
			}
			case WM_INPUT:
			{
				HWindow* window = reinterpret_cast<HWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
				UINT size{ 0u };
				H_ASSERT(::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) != -1, "Failed to get raw input data.");
				std::vector<BYTE> buffer(size);
				H_ASSERT(::GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER)) == size, "Failed to get raw input data.");
				auto& input = reinterpret_cast<const RAWINPUT&>(*buffer.data());

				if (input.header.dwType == RIM_TYPEMOUSE && (input.data.mouse.lLastX != 0 || input.data.mouse.lLastY != 0))
				{
					if(window->mMessageCallback)
					{
						HMouseRawInputEvent event{ { input.data.mouse.lLastX, input.data.mouse.lLastY } };
						window->mMessageCallback(event);
					}
				}

				break;
			}
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

}