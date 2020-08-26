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
		 * @param VSync 
		 */
		void EndFrame(bool VSync);

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
		 * Get D3D11 Device
		 * @return 
		 */
		ID3D11Device* GetDeviceInternal() const;

		/**
		 * Get D3D11 Context
		 * @return 
		 */
		ID3D11DeviceContext* GetContextInternal() const;

	private:
		friend class HBindable;

		void InitializeDepthStencilView(uint32_t Width, uint32_t Height);
		void InitializeRenderTargetView();
		void InitializeViewport(uint32_t Width, uint32_t Height);
		
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice{ nullptr };
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapchain{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView{ nullptr };
		
		ScopedPointer<class HDXGIInfoQueue> mDXGIInfoQueue{ nullptr };
	};

}
