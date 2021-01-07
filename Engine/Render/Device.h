#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	class Device final {
	public:
		HL_DISABLE_COPY(Device);

		Device();
		~Device();

		inline D3D_FEATURE_LEVEL GetFeatureLevel() const noexcept { return mFeatureLevel; }
		inline ID3D11Device* GetDevice() const noexcept { return mDevice.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() const noexcept { return mDeviceContext.Get(); }
		inline IDXGIFactory* GetDxgiFactory() const noexcept { return mDxgiFactory.Get(); }

	private:
		D3D_FEATURE_LEVEL mFeatureLevel = D3D_FEATURE_LEVEL_11_0;
		
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
		
		Microsoft::WRL::ComPtr<IDXGIDevice> mDxgiDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> mDxgiAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> mDxgiFactory = nullptr;

#ifdef _DEBUG
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> mDxgiInfoQueue = nullptr;
#endif

	};

}