#include "HowlitePCH.h"
#include "MouseEvent.h"

namespace Howlite {

	HMouseMovedEvent::HMouseMovedEvent(const HInputSystem::HPoint& Position) :
		mPosition{ Position }
	{

	}

	HMouseMovedEvent::~HMouseMovedEvent()
	{

	}

	int HMouseMovedEvent::GetX() const noexcept
	{
		return mPosition.X;
	}

	int HMouseMovedEvent::GetY() const noexcept
	{
		return mPosition.Y;
	}

	HInputSystem::HPoint HMouseMovedEvent::GetPoint() const noexcept
	{
		return mPosition;
	}

	HMouseButtonEvent::HMouseButtonEvent(unsigned char Button) :
		mButton{ Button }
	{

	}

	HMouseButtonEvent::~HMouseButtonEvent()
	{

	}

	unsigned char HMouseButtonEvent::GetButton() const noexcept
	{
		return mButton;
	}

	HMouseButtonPressedEvent::HMouseButtonPressedEvent(unsigned char Button) :
		HMouseButtonEvent{ Button }
	{

	}

	HMouseButtonPressedEvent::~HMouseButtonPressedEvent()
	{

	}

	HMouseButtonReleasedEvent::HMouseButtonReleasedEvent(unsigned char Button) :
		HMouseButtonEvent{ Button }
	{

	}

	HMouseButtonReleasedEvent::~HMouseButtonReleasedEvent()
	{

	}

	HMouseRawInputEvent::HMouseRawInputEvent(const HInputSystem::HPoint& RawInput) :
		HMouseMovedEvent{ RawInput }
	{

	}

	HMouseRawInputEvent::~HMouseRawInputEvent()
	{

	}

}