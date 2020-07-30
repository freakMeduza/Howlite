#pragma once

#include "Common/Color.h"

namespace Howlite {

	class HGraphicSystem {
	public:
		HGraphicSystem(HWND hWnd, uint32_t Width, uint32_t Height);
		~HGraphicSystem();

		HGraphicSystem(HGraphicSystem&&) = delete;
		HGraphicSystem(const HGraphicSystem&) = delete;
		
		HGraphicSystem& operator=(HGraphicSystem&&) = delete;
		HGraphicSystem& operator=(const HGraphicSystem&) = delete;

		/**
		 * Begin Frame
		 * @param Color 
		 */
		void BeginFrame(HColor Color);

		/** 
		 * End Frame
		 */
		void EndFrame();

		/**
		 * Draw Indexed
		 * @param IndexCount 
		 */
		void DrawIndexed(UINT IndexCount);

		/**
		 * Resize Buffers
		 * @param Width 
		 * @param Height 
		 */
		void ResizeBuffers(uint32_t Width, uint32_t Height);

		/**
		 * Set VSync Is Enabled
		 * @param VSyncEnabled 
		 * @return 
		 */
		void SetVSyncEnabled(bool VSyncEnabled) noexcept;

		/**
		 * Is VSync Is Enabled
		 * @return 
		 */
		bool IsVSyncEnabled() const noexcept;

		/** 
		 * Get UI System Instance
		 */
		class HUISystem& GetUISystemInstance();

	private:
		friend class HBindable;

		void CreateDeviceAndSwapchain(HWND hWnd, uint32_t Width, uint32_t Height);
		void CreateViewport(uint32_t Width, uint32_t Height);
		void CreateRenderTargetView();

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice{ nullptr };
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapchain{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView{ nullptr };

		ScopedPointer<class HUISystem> mUISystem{ nullptr };
		ScopedPointer<class HDXGIInfoQueue> mDXGIInfoQueue{ nullptr };

		bool mVSyncIsEnabled{ false };
	};

}
