#pragma once

#include "Event.h"
#include "Input/InputSystem.h"

namespace Howlite {

	class HMouseMovedEvent : public IEvent {
	public:
		H_GENERATE_EVENT(EEventType::MouseMoved)

		HMouseMovedEvent(const HInputSystem::HPoint& Position);
		~HMouseMovedEvent() override;

		/**
		 * Get Mouse X Coordinate
		 * @return 
		 */
		int GetX() const noexcept;
		
		/**
		 * Get Mouse Y Coordinate
		 * @return 
		 */
		int GetY() const noexcept;

		/**
		 * Get Mouse Position
		 * @return 
		 */
		HInputSystem::HPoint GetPoint() const noexcept;
	
	private:
		HInputSystem::HPoint mPosition;

	};

	class HMouseRawInputEvent : public HMouseMovedEvent {
	public:
		H_GENERATE_EVENT(EEventType::MouseRawInput)

		HMouseRawInputEvent(const HInputSystem::HPoint& RawInput);
		~HMouseRawInputEvent() override;
	};

	class HMouseEnteredEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseEntered)

		HMouseEnteredEvent() = default;
		~HMouseEnteredEvent() override = default;
		
	};

	class HMouseLeavedEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseLeaved)

		HMouseLeavedEvent() = default;
		~HMouseLeavedEvent() override = default;

	};

	class HMouseScrolledUpEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseScrolledUp)

		HMouseScrolledUpEvent() = default;
		~HMouseScrolledUpEvent() override = default;

	};

	class HMouseScrolledDownEvent : public IEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseScrolledDown)

		HMouseScrolledDownEvent() = default;
		~HMouseScrolledDownEvent() override = default;

	};

	class HMouseButtonEvent : public IEvent {
	public:
		HMouseButtonEvent(unsigned char Button);
		~HMouseButtonEvent() override;

		/**
		 * Get Mouse Button Code
		 * @return 
		 */
		unsigned char GetButton() const noexcept;

	protected:
		unsigned char mButton{ 0u };
	};

	class HMouseButtonPressedEvent : public HMouseButtonEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseButtonPressed)

		HMouseButtonPressedEvent(unsigned char Button);
		~HMouseButtonPressedEvent() override;

	};

	class HMouseButtonReleasedEvent : public HMouseButtonEvent
	{
	public:
		H_GENERATE_EVENT(EEventType::MouseButtonReleased)

		HMouseButtonReleasedEvent(unsigned char Button);
		~HMouseButtonReleasedEvent() override;

	};

}
