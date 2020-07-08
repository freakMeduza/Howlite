#pragma once

#include "Event.h"

namespace Howlite {

	class HKeyEvent : public IHEvent {
	public:
		HKeyEvent(unsigned char Key);
		~HKeyEvent() override;

		/**
		 * Get Key Code
		 * @return 
		 */
		unsigned char GetKey() const noexcept;

	protected:
		unsigned char mKey{ 0u };

	};

	class HKeyPressedEvent : public HKeyEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::KeyPressed)

		HKeyPressedEvent(unsigned char Key);
		~HKeyPressedEvent() override;

	};

	class HKeyReleasedEvent : public HKeyEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::KeyReleased)

		HKeyReleasedEvent(unsigned char Key);
		~HKeyReleasedEvent() override;

	};

	class HKeyTypedEvent : public HKeyEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::KeyTyped)

		HKeyTypedEvent(unsigned char Key);
		~HKeyTypedEvent() override;

	};
}
