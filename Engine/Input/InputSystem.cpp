#include "InputSystem.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Event/EventManager.h"

namespace Howlite {

	InputSystem::InputSystem() : TSystem { Priority::Hight }
	{
		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Subscribe<WindowCreateEvent>("InputSystem::OnWindowCreated", [this](const WindowCreateEvent* InEvent) { OnWindowCreated(InEvent); });
		eventManager->Subscribe<WindowDestroyEvent>("InputSystem::WindowDestroyEvent", [this](const WindowDestroyEvent* InEvent) { OnWindowDestroyed(InEvent); });
		eventManager->Subscribe<MouseEnterEvent>("InputSystem::OnMouseEntered", [this](const MouseEnterEvent* InEvent) { OnMouseEntered(InEvent); });
		eventManager->Subscribe<MouseLeaveEvent>("InputSystem::OnMouseLeaved", [this](const MouseLeaveEvent* InEvent) { OnMouseLeaved(InEvent); });
	}
	
	InputSystem::~InputSystem()
	{
		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Unsubscribe<WindowCreateEvent>("InputSystem::OnWindowCreated");
		eventManager->Unsubscribe<WindowDestroyEvent>("InputSystem::WindowDestroyEvent");
		eventManager->Unsubscribe<MouseEnterEvent>("InputSystem::OnMouseEntered");
		eventManager->Unsubscribe<MouseLeaveEvent>("InputSystem::OnMouseLeaved");
	}

	void InputSystem::OnWindowCreated(const WindowCreateEvent* InEvent)
	{
		if (!mHandle && !mMouse.mIsRawInputInitialized)
		{
			RAWINPUTDEVICE rawInputDevice;
			ZeroMemory(&rawInputDevice, sizeof(rawInputDevice));
			rawInputDevice.usUsagePage = 0x01;
			rawInputDevice.usUsage = 0x02;
			rawInputDevice.dwFlags = RIDEV_INPUTSINK;
			rawInputDevice.hwndTarget = InEvent->mHandle;
			::RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice));
			mMouse.mIsRawInputInitialized = true;
			mHandle = InEvent->mHandle;
		}
	}

	void InputSystem::OnWindowDestroyed(const WindowDestroyEvent* InEvent)
	{
		mHandle = nullptr;
		mMouse.mIsRawInputInitialized = false;
	}

	void InputSystem::OnMouseEntered(const MouseEnterEvent* InEvent)
	{
		HL_UNUSED(InEvent);
		mMouse.mIsInWindow = true;
	}

	void InputSystem::OnMouseLeaved(const MouseLeaveEvent* InEvent)
	{
		HL_UNUSED(InEvent);
		mMouse.mIsInWindow = false;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return mIsInWindow;
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