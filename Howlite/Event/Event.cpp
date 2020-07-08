#include "HowlitePCH.h"
#include "Event.h"

namespace Howlite {

    HEventDispatcher::HEventDispatcher(IHEvent& Event) :
		mEvent{ Event }
    {

    }

    HEventDispatcher::~HEventDispatcher()
    {

    }

}