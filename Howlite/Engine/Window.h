#pragma once

namespace Howlite {

	class HWindow {
	public:
		HWindow(HINSTANCE Instance, const char* Title, uint32_t Width, uint32_t Height);
		~HWindow();

		HWindow(HWindow&&) = delete;
		HWindow(const HWindow&) = delete;

		HWindow& operator=(HWindow&&) = delete;
		HWindow& operator=(const HWindow&) = delete;

		/**
		 * Process Windows Messages
		 */
		static void ProcessMessages();

		/**
		 * Set Windows Message Callback
		 * @param Callback 
		 */
		void SetMessageCallback(std::function<void(class IEvent&)> MesageCallback) noexcept;

		/**
		 * Get Window Handler
		 * @return 
		 */
		HWND GetWindowHandler() const noexcept;

		/**
		 * Get Window Width
		 * @return 
		 */
		uint32_t GetWidth() const noexcept;

		/**
		 * Get Window Height
		 * @return 
		 */
		uint32_t GetHeight() const noexcept;
		
	private:
		void RegisterWindowClass();
		void UnregisterWindowClass();

		static LRESULT CALLBACK HandleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		std::function<void(class IEvent&)> mMessageCallback;
		
		static constexpr auto* mWindowClassName = TEXT("Howlite.Engine.Window");

		HWND mHandler;
		HINSTANCE mInstance;
		
		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };

	};

}
