#include "HowlitePCH.h"
#include "Tick.h"

namespace Howlite {

	HTick::HTick(float Time) :
		mTime{ Time }
	{

	}

	float HTick::GetSeconds() const noexcept
	{
		return mTime;
	}

	float HTick::GetMilliseconds() const noexcept
	{
		return mTime * 1000.0f;
	}

}