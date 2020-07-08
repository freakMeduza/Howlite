#pragma once

#include "Event.h"

namespace Howlite {

	class HWindowResizedEvent : public IHEvent {
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::WindowResized)
		
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

	class HWindowClosedEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::WindowClosed)

		HWindowClosedEvent() = default;
		~HWindowClosedEvent() override = default;
	};

	class HWindowFocusedEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::WindowFocused)

		HWindowFocusedEvent() = default;
		~HWindowFocusedEvent() override = default;
	};

	class HWindowUnfocusedEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::WindowUnfocused)

		HWindowUnfocusedEvent() = default;
		~HWindowUnfocusedEvent() override = default;
	};

}
