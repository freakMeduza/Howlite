#include "HowlitePCH.h"
#include "KeyboardEvent.h"

namespace Howlite {

	HKeyEvent::HKeyEvent(unsigned char Key) :
		mKey{ Key }
	{

	}

	HKeyEvent::~HKeyEvent()
	{

	}

	unsigned char HKeyEvent::GetKey() const noexcept
	{
		return mKey;
	}

	HKeyPressedEvent::HKeyPressedEvent(unsigned char Key) :
		HKeyEvent{ Key }
	{

	}

	HKeyPressedEvent::~HKeyPressedEvent()
	{

	}

	HKeyReleasedEvent::HKeyReleasedEvent(unsigned char Key) :
		HKeyEvent{ Key }
	{

	}

	HKeyReleasedEvent::~HKeyReleasedEvent()
	{
		
	}

	HKeyTypedEvent::HKeyTypedEvent(unsigned char Key) :
		HKeyEvent{ Key }
	{

	}

	HKeyTypedEvent::~HKeyTypedEvent()
	{

	}

}