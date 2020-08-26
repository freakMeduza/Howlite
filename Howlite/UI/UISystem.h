#pragma once

namespace Howlite {
	
	enum class EUIStyle
	{
		Dark
	};

	class HUISystem {
	public:
		HUISystem(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* Context);
		~HUISystem();

		HUISystem(HUISystem&&) = delete;
		HUISystem(const HUISystem&) = delete;
	
		HUISystem& operator=(HUISystem&&) = delete;
		HUISystem& operator=(const HUISystem&) = delete;

		/** 
		 * Begin Frame
		 */
		void BeginFrame();

		/** 
		 * End Frame
		 */
		void EndFrame();

		/**
		 * Set Is Enabled
		 * @param IsEnabled 
		 */
		void SetIsEnabled(bool IsEnabled) noexcept;

		/**
		 * Is Enabled
		 * @return 
		 */
		bool IsEnabled() const noexcept;

		/**
		 * Handle Windows Message
		 * @param hWnd 
		 * @param Msg 
		 * @param wParam 
		 * @param lParam 
		 * @return 
		 */
		static LRESULT HandleMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	private:
		void InitializeUIStyle(EUIStyle Style);

		bool mIsEnabled{ false };

	};

}