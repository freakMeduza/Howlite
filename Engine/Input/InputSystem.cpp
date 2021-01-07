#include "InputSystem.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Event/EventManager.h"

namespace Howlite {

	InputSystem::InputSystem() : TSystem { Priority::Hight }
	{
		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Subscribe<WindowCreateEvent>("InputSystem::OnWindowCreated", [this](const WindowCreateEvent* InEvent) { OnWindowCreated(InEvent); });
	}
	
	InputSystem::~InputSystem()
	{
		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Unsubscribe<WindowCreateEvent>("InputSystem::OnWindowCreated");
	}

	void InputSystem::OnWindowCreated(const WindowCreateEvent* InEvent)
	{
		if (!mMouse.mIsRawInputInitialized)
		{
			RAWINPUTDEVICE rawInputDevice;
			ZeroMemory(&rawInputDevice, sizeof(rawInputDevice));
			rawInputDevice.usUsagePage = 0x01;
			rawInputDevice.usUsage = 0x02;
			rawInputDevice.dwFlags = RIDEV_INPUTSINK;
			rawInputDevice.hwndTarget = InEvent->mHandle;
			::RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));
			mMouse.mIsRawInputInitialized = true;
		}
	}

	bool Mouse::IsLeftButtonPressed() const noexcept
	{
		return (::GetKeyState(VK_LBUTTON) & 0x8000) != 0;
	}

	bool Mouse::IsMiddleButtonPressed() const noexcept
	{
		return (::GetKeyState(VK_MBUTTON) & 0x8000) != 0;
	}

	bool Mouse::IsRightButtonPressed() const noexcept
	{
		return (::GetKeyState(VK_RBUTTON) & 0x8000) != 0;
	}

	Mouse::Position Mouse::GetPosition() const noexcept
	{
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(::GetActiveWindow(), &point);
		return { point.x, point.y };
	}

	bool Keyboard::IsKeyPressed(const int InKeyCode) const noexcept
	{
		return (::GetKeyState(InKeyCode) & 0x8000) != 0;
	}

}