#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	class IEvent {
	public:
		HL_INTERFACE_DEFAULT(IEvent);

		virtual EventTypeId GetEventTypeId() const = 0;

	};

	template<typename EventType>
	class TEvent : public IEvent {
	public:
		HL_CLASS_DEFAULT(TEvent);
		// Inherited via IEvent
		virtual EventTypeId GetEventTypeId() const override
		{
			return STATIC_EVENT_TYPE_ID;
		}

		static const EventTypeId STATIC_EVENT_TYPE_ID;

	};

	template<typename EventType> const EventTypeId TEvent<EventType>::STATIC_EVENT_TYPE_ID = typeid(EventType).hash_code();

}