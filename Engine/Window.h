#pragma once

#include "Event/EventManager.h"

namespace Howlite {

	class Window {
		static size_t WINDOW_INSTANCE_COUNT;

		static constexpr char* WINDOW_CLASS_NAME = "HowliteWindowClass";

	public:
		HL_CLASS_DISABLE_COPY(Window);

		enum class Style {
			Empty      = 0,      // no border, no title bar
			Title      = 1 << 0, // title bar + fixed border
			Resize     = 1 << 1, // title bar + resizable border + min/max button
			Close      = 1 << 2, // title bar + close button
			Fullscreen = 1 << 3, // fullscreen
		};

		bool Init(const uint32_t InWidth, const uint32_t InHeight);
		bool Init(const uint32_t InWidth, const uint32_t InHeight, const std::string& InTitle);
		bool Init(const uint32_t InWidth, const uint32_t InHeight, const std::string& InTitle, const Style InStyle);
		void Terminate();

		inline uint32_t GetWidth() const noexcept { return mWidth; }
		inline uint32_t GetHeight() const noexcept { return mHeight; }

		using MessageCallback = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

		void SetMessageCallback(MessageCallback&& InMessageCallback);

	private:
		void RegisterWindowClass();
		void UnregisterWindowClass();
		void SetupFullScreenMode(const uint32_t InWidth, const uint32_t InHeight);

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		HWND mHandle = nullptr;
		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
		bool mIsFullScreen = false;
		MessageCallback mMessageCallback;
	};

	inline Window::Style operator&(Window::Style Left, Window::Style Right)
	{
		return static_cast<Window::Style>((uint8_t)Left & (uint8_t)Right);
	}

	inline Window::Style operator|(Window::Style Left, Window::Style Right)
	{
		return static_cast<Window::Style>((uint8_t)Left | (uint8_t)Right);
	}
}