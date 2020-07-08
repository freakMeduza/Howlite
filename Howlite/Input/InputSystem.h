#pragma once

namespace Howlite {

	class HInputSystem {
	public:
		HInputSystem() = default;
		~HInputSystem() = default;

		struct HPoint
		{
			int X{ 0 };
			int Y{ 0 };
		};

		HInputSystem(HInputSystem&&) = delete;
		HInputSystem(const HInputSystem&) = delete;

		HInputSystem& operator=(HInputSystem&&) = delete;
		HInputSystem& operator=(const HInputSystem&) = delete;

		/**
		 * Is Key Pressed
		 * @param KeyCode 
		 * @return 
		 */
		bool IsKeyPressed(unsigned char KeyCode) const noexcept;

		/**
		 * Is Left Mouse Button Pressed
		 * @return 
		 */
		bool IsLeftMouseButtonPressed() const noexcept;

		/**
		 * Is Right Mouse Button Pressed
		 * @return 
		 */
		bool IsRightMouseButtonPressed() const noexcept;

		/**
		 * Is Middle Mouse Button Pressed
		 * @return 
		 */
		bool IsMiddleMouseButtonPressed() const noexcept;

		/**
		 * Get Mouse Position
		 * @return 
		 */
		HPoint GetMousePosition() const noexcept;

		/**
		 * Set Whenever Mouse In Window
		 * @param IsInWindow 
		 */
		void SetMouseInWindow(bool InWindow);

		/**
		 * Is Mouse In Window
		 * @return 
		 */
		bool IsMouseInWindow() const noexcept;

	private:
		bool mMouseInWindow{ false };

	};

}