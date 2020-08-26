#pragma once

#include "Event.h"

namespace Howlite {

	class HKeyEvent : public IEvent {
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
		H_GENERATE_EVENT(EEventType::KeyPressed)

		HKeyPressedEvent(unsigned char Key);
		~HKeyPressedEvent() override;

	};

	class HKeyReleasedEvent : public HKeyEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::KeyReleased)

		HKeyReleasedEvent(unsigned char Key);
		~HKeyReleasedEvent() override;

	};

	class HKeyTypedEvent : public HKeyEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::KeyTyped)

		HKeyTypedEvent(unsigned char Key);
		~HKeyTypedEvent() override;

	};
}
