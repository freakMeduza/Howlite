#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	class SwapChain final {
	public:
		HL_DISABLE_COPY(SwapChain);

		SwapChain(const class Device* InDevice, HWND InHandle, const uint32_t InWidth, const uint32_t InHeight);
		~SwapChain();

		void ClearDepthStencilView();
		void ClearRenderTargetView(const std::array<float, 4>& InClearColor);
		void Present(const bool IsVerticalSncEnabled);
		void Bind();

	private:
		const class Device* mDevice = nullptr;

		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;

	};

}