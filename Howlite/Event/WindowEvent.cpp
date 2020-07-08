#include "HowlitePCH.h"
#include "WindowEvent.h"

namespace Howlite {

	HWindowResizedEvent::HWindowResizedEvent(uint32_t Width, uint32_t Height) :
		mWidth{ Width }, mHeight{ Height }
	{

	}

	HWindowResizedEvent::~HWindowResizedEvent()
	{

	}

	uint32_t HWindowResizedEvent::GetWidth() const noexcept
	{
		return mWidth;
	}

	uint32_t HWindowResizedEvent::GetHeight() const noexcept
	{
		return mHeight;
	}

}