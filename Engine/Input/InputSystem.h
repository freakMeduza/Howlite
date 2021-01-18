#pragma once

#include "Engine/ECS/System.h"
#include "Engine/Event/Event.h"

namespace Howlite {

	class MouseRawInputEvent : public TEvent<MouseRawInputEvent> {
	public:
		int mDeltaX = 0;
		int mDeltaY = 0;

		MouseRawInputEvent(const int InDeltaX, const int InDeltaY) :
			mDeltaX{ InDeltaX }, mDeltaY{ InDeltaY }
		{

		}
	};

	class MouseEnterEvent : public TEvent<MouseEnterEvent> {
	public:
		HWND mHandle;

		MouseEnterEvent(HWND InHandle) :
			mHandle{ InHandle }
		{

		}
	};

	class MouseLeaveEvent : public TEvent<MouseLeaveEvent> {
	public:
		HWND mHandle;

		MouseLeaveEvent(HWND InHandle) :
			mHandle{ InHandle }
		{

		}
	};

	class MouseMoveEvent : public TEvent<MouseMoveEvent> {
	public:
		int mPositionX;
		int mPositionY;

		MouseMoveEvent(const int InPositionX, const int InPositionY) :
			mPositionX{ InPositionX }, mPositionY{ InPositionY }
		{

		}
	};

	struct Mouse {
		HL_CLASS_DEFAULT(Mouse);

		using Position = std::pair<int, int>;

		bool IsInWindow() const noexcept;
		bool IsLeftButtonPressed() const noexcept;
		bool IsMiddleButtonPressed() const noexcept;
		bool IsRightButtonPressed() const noexcept;
		Position GetPosition() const noexcept;

		bool mIsRawInputInitialized = false;
		bool mIsInWindow = false;
	};

	struct Keyboard {
		HL_CLASS_DEFAULT(Keyboard);

		bool IsKeyPressed(const int InKeyCode) const noexcept;
	};

	class InputSystem : public TSystem<InputSystem> {
	public:
		HL_DISABLE_COPY(InputSystem);

		InputSystem();
		~InputSystem();

		inline const Mouse& GetMouse() const noexcept { return mMouse; }
		inline const Keyboard& GetKeyboard() const noexcept { return mKeyboard; }
		inline const HWND GetWindowHandle() const noexcept { return mHandle; }

	private:
		void OnWindowCreated(const class WindowCreateEvent* InEvent);
		void OnWindowDestroyed(const class WindowDestroyEvent* InEvent);
		void OnMouseEntered(const MouseEnterEvent* InEvent);
		void OnMouseLeaved(const MouseLeaveEvent* InEvent);

		Mouse mMouse;
		Keyboard mKeyboard;

		HWND mHandle = nullptr;

	};

}

// Constant          Note
// VK_ESCAPE   
// VK_1             On main keyboard 
// VK_2             On main keyboard 
// VK_3             On main keyboard 
// VK_4             On main keyboard 
// VK_5             On main keyboard 
// VK_6             On main keyboard 
// VK_7             On main keyboard 
// VK_8             On main keyboard 
// VK_9             On main keyboard 
// VK_0             On main keyboard 
// VK_MINUS         On main keyboard 
// VK_EQUALS        On main keyboard 
// VK_BACK          The backspace key  
// VK_TAB   
// VK_Q   
// VK_W   
// VK_E   
// VK_R   
// VK_T   
// VK_Y   
// VK_U   
// VK_I   
// VK_O   
// VK_P   
// VK_LBRACKET      The [ key  
// VK_RBRACKET      The ] key  
// VK_RETURN        enter key on main keyboard  
// VK_LCONTROL      Left ctrl key  
// VK_A   
// VK_S   
// VK_D   
// VK_F   
// VK_G   
// VK_H   
// VK_J   
// VK_K   
// VK_L   
// VK_SEMICOLON   
// VK_APOSTROPHE   
// VK_GRAVE          Grave accent (`) key  
// VK_LSHIFT         Left shift key  
// VK_BACKSLASH   
// VK_Z   
// VK_X   
// VK_C   
// VK_V   
// VK_B   
// VK_N   
// VK_M   
// VK_COMMA   
// VK_PERIOD         On main keyboard  
// VK_SLASH          Forward slash on main keyboard 
// VK_RSHIFT         Right shift key  
// VK_MULTIPLY       The * key on numeric keypad  
// VK_LMENU          Left alt key  
// VK_SPACE          spacebar  
// VK_CAPITAL        caps lock key  
// VK_F1   
// VK_F2   
// VK_F3   
// VK_F4   
// VK_F5   
// VK_F6   
// VK_F7   
// VK_F8   
// VK_F9   
// VK_F10   
// VK_NUMLOCK   
// VK_SCROLL         scroll lock 
// VK_NUMPAD7   
// VK_NUMPAD8   
// VK_NUMPAD9   
// VK_SUBTRACT       minus sign on numeric keypad  
// VK_NUMPAD4   
// VK_NUMPAD5   
// VK_NUMPAD6   
// VK_ADD            plus sign on numeric keypad  
// VK_NUMPAD1   
// VK_NUMPAD2   
// VK_NUMPAD3   
// VK_NUMPAD0   
// VK_DECIMAL        period (decimal point) on numeric keypad  
// VK_F11   
// VK_F12   
// VK_F13  
// VK_F14  
// VK_F15  
// VK_KANA           On Japanese keyboard 
// VK_CONVERT        On Japanese keyboard 
// VK_NOCONVERT      On Japanese keyboard 
// VK_YEN            On Japanese keyboard 
// VK_NUMPADEQUALS   On numeric keypad (NEC PC98) 
// VK_CIRCUMFLEX     On Japanese keyboard 
// VK_AT             On Japanese keyboard 
// VK_COLON          On Japanese keyboard 
// VK_UNDERLINE      On Japanese keyboard 
// VK_KANJI          On Japanese keyboard 
// VK_STOP           On Japanese keyboard 
// VK_AX             On Japanese keyboard 
// VK_UNLABELED      On Japanese keyboard 
// VK_NUMPADENTER   
// VK_RCONTROL       Right ctrl key  
// VK_NUMPADCOMMA    comma on NEC PC98 numeric keypad 
// VK_DIVIDE         Forward slash on numeric keypad  
// VK_SYSRQ   
// VK_RMENU          Right alt key  
// VK_HOME   
// VK_UP             up arrow  
// VK_PRIOR          page up  
// VK_LEFT           left arrow  
// VK_RIGHT          right arrow  
// VK_END   
// VK_DOWN           down arrow  
// VK_NEXT           page down  
// VK_INSERT   
// VK_DELETE   
// VK_LWIN           Left Windows key  
// VK_RWIN           Right Windows key  
// VK_APPS           Application key  
// VK_PAUSE
