#pragma once

#include "Event.h"
#include "Input/InputSystem.h"

namespace Howlite {

	class HMouseMovedEvent : public IHEvent {
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseMoved)

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
		H_DECLARE_EVENT_TYPE(EHEventType::MouseRawInput)

		HMouseRawInputEvent(const HInputSystem::HPoint& RawInput);
		~HMouseRawInputEvent() override;
	};

	class HMouseEnteredEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseEntered)

		HMouseEnteredEvent() = default;
		~HMouseEnteredEvent() override = default;
		
	};

	class HMouseLeavedEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseLeaved)

		HMouseLeavedEvent() = default;
		~HMouseLeavedEvent() override = default;

	};

	class HMouseScrolledUpEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseScrolledUp)

		HMouseScrolledUpEvent() = default;
		~HMouseScrolledUpEvent() override = default;

	};

	class HMouseScrolledDownEvent : public IHEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseScrolledDown)

		HMouseScrolledDownEvent() = default;
		~HMouseScrolledDownEvent() override = default;

	};

	class HMouseButtonEvent : public IHEvent {
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
		H_DECLARE_EVENT_TYPE(EHEventType::MouseButtonPressed)

		HMouseButtonPressedEvent(unsigned char Button);
		~HMouseButtonPressedEvent() override;

	};

	class HMouseButtonReleasedEvent : public HMouseButtonEvent
	{
	public:
		H_DECLARE_EVENT_TYPE(EHEventType::MouseButtonReleased)

		HMouseButtonReleasedEvent(unsigned char Button);
		~HMouseButtonReleasedEvent() override;

	};

}
