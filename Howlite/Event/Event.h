#pragma once

namespace Howlite {

	enum class EEventType
	{
		Undefined,
		WindowClosed,
		WindowResized,
		WindowFocused,
		WindowUnfocused,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseScrolledUp,
		MouseScrolledDown,
		MouseEntered,
		MouseLeaved,
		MouseMoved,
		MouseRawInput
	};

	class IEvent {
	public:
		virtual ~IEvent() = default;

		/**
		 * Get Event Type
		 * @return 
		 */
		virtual EEventType GetEventType() const noexcept = 0;

		/**
		 * Get Event Name
		 * @return 
		 */
		virtual const char* GetEventName() const noexcept = 0;
		
		bool Handled{ false };

	};

	class HEventDispatcher {
	public:
		HEventDispatcher(IEvent& Event);
		~HEventDispatcher();

		/**
		 * Dispatch Event
		 * @param EventCallback 
		 * @return true 
		 * @return false 
		 */
		template<typename Event, typename Callback>
		bool Dispatch(Callback EventCallback)
		{
			if(mEvent.GetEventType() == Event::GetStaticEventType())
			{
				mEvent.Handled = EventCallback(static_cast<Event&>(mEvent));

				return true;
			}

			return false;
		}

	private:
		IEvent& mEvent;

	};

}