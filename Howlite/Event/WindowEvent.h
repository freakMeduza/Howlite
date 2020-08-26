#pragma once

#include "Event.h"

namespace Howlite {

	class HWindowResizedEvent : public IEvent {
	public:
		H_GENERATE_EVENT(EEventType::WindowResized)
		
		HWindowResizedEvent(uint32_t Width, uint32_t Height);
		~HWindowResizedEvent() override;

		/**
		 * Get Width
		 * @return 
		 */
		uint32_t GetWidth() const noexcept;

		/**
		 * Get Height
		 * @return 
		 */
		uint32_t GetHeight() const noexcept;

	private:
		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };

	};

	class HWindowClosedEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::WindowClosed)

		HWindowClosedEvent() = default;
		~HWindowClosedEvent() override = default;
	};

	class HWindowFocusedEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::WindowFocused)

		HWindowFocusedEvent() = default;
		~HWindowFocusedEvent() override = default;
	};

	class HWindowUnfocusedEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::WindowUnfocused)

		HWindowUnfocusedEvent() = default;
		~HWindowUnfocusedEvent() override = default;
	};

}
