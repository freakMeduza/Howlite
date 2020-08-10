#pragma once

namespace Howlite {
	
	using HUIComponent = SharedPointer<std::function<void()>>;

	class HUISystem {
	public:

		HUISystem(HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* Context);
		~HUISystem();

		HUISystem(HUISystem&&) = delete;
		HUISystem(const HUISystem&) = delete;
	
		HUISystem& operator=(HUISystem&&) = delete;
		HUISystem& operator=(const HUISystem&) = delete;

		/**
		 * Create UI Component
		 * @param Function 
		 * @return 
		 */
		template<typename Functor>
		static HUIComponent CreateUIComponent(Functor Function)
		{
			return HUIComponent{ new std::function<void()>(Function) };
		}

		/** 
		 * Begin Frame
		 */
		void BeginFrame();

		/** 
		 * End Frame
		 */
		void EndFrame();

		/** 
		 * Draw UI Frame
		 */
		void Draw();

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

		/**
		 * Bind UI Component
		 * @param UIComponent - Lambda
		 * @return 
		 */
		void BindUIComponent(HUIComponent UIComponent) noexcept;

		/**
		 * Unbind UI Component
		 * @param UIComponent 
		 * @return 
		 */
		void UnbindUIComponent(HUIComponent UIComponent) noexcept;

	private:
		bool mIsEnabled{ false };

		std::set<HUIComponent> mUIComponents;

	};

}