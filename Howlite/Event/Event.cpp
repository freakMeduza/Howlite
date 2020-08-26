#include "HowlitePCH.h"
#include "Event.h"

namespace Howlite {

    HEventDispatcher::HEventDispatcher(IEvent& Event) :
		mEvent{ Event }
    {

    }

    HEventDispatcher::~HEventDispatcher()
    {

    }

}