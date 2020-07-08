#include "HowlitePCH.h"
#include "InputSystem.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"

namespace Howlite {

	bool HInputSystem::IsKeyPressed(unsigned char KeyCode) const noexcept
	{
		return ::GetKeyState(KeyCode) < 0;
	}
	
	bool HInputSystem::IsLeftMouseButtonPressed() const noexcept
	{
		return ::GetKeyState(VK_LBUTTON) < 0;
	}

	bool HInputSystem::IsRightMouseButtonPressed() const noexcept
	{
		return ::GetKeyState(VK_RBUTTON) < 0;
	}

	bool HInputSystem::IsMiddleMouseButtonPressed() const noexcept
	{
		return ::GetKeyState(VK_MBUTTON) < 0;
	}

	HInputSystem::HPoint HInputSystem::GetMousePosition() const noexcept
	{
		POINT point;

		::GetCursorPos(&point);
		::ScreenToClient(HEngine::GetInstance().GetWindowInstance().GetWindowHandler(), &point);

		return { point.x, point.y };
	}

	void HInputSystem::SetMouseInWindow(bool InWindow)
	{
		mMouseInWindow = InWindow;
	}

	bool HInputSystem::IsMouseInWindow() const noexcept
	{
		return mMouseInWindow;
	}

}
