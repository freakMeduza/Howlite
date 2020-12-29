#pragma once

#include "Engine/ECS/System.h"

namespace Howlite {

	class RenderSystem final : public TSystem<RenderSystem>	{
	public:
		HL_DISABLE_COPY(RenderSystem);

		RenderSystem();
		~RenderSystem();

		inline ID3D11Device* GetDevice() const noexcept { return mDevice.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() const noexcept { return mDeviceContext.Get(); }
		inline D3D_FEATURE_LEVEL GetFeatureLevel() const noexcept { return mFeatureLevel; }

	private:
		D3D_FEATURE_LEVEL mFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
		// dxgi
		Microsoft::WRL::ComPtr<IDXGIDevice> mDxgiDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> mDxgiAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> mDxgiFactory = nullptr;

#ifdef _DEBUG
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> mDxgiInfoQueue = nullptr;
#endif

	};

}