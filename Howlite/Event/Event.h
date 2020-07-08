#pragma once

namespace Howlite {

	enum class EHEventType
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

	class IHEvent {
	public:
		virtual ~IHEvent() = default;

		/**
		 * Get Event Type
		 * @return 
		 */
		virtual EHEventType GetEventType() const noexcept = 0;

		/**
		 * Get Event Name
		 * @return 
		 */
		virtual const char* GetEventName() const noexcept = 0;
		
		bool Handled{ false };

	};

	class HEventDispatcher {
	public:
		HEventDispatcher(IHEvent& Event);
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
		IHEvent& mEvent;

	};

}