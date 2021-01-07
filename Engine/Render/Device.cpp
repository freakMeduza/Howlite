#include "Device.h"

namespace Howlite {

	Device::Device()
	{
		D3D_DRIVER_TYPE driverTypeList[]
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		D3D_FEATURE_LEVEL featureLevelList[] =
		{
			D3D_FEATURE_LEVEL_11_0
		};

		UINT flags = 0;

#ifdef _DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;
		for (auto i = 0; i < ARRAYSIZE(driverTypeList); ++i)
		{
			result = D3D11CreateDevice(nullptr, driverTypeList[i], nullptr, flags, featureLevelList,
				ARRAYSIZE(featureLevelList), D3D11_SDK_VERSION, &mDevice, &mFeatureLevel, &mDeviceContext);

			if (SUCCEEDED(result)) break;
		}

		HL_ASSERT(mDevice, "Failed to intialize device");
		// initialize dxgi stuff
		mDevice->QueryInterface(__uuidof(IDXGIDevice), &mDxgiDevice);
		mDxgiDevice->GetParent(__uuidof(IDXGIAdapter), &mDxgiAdapter);
		mDxgiAdapter->GetParent(__uuidof(IDXGIFactory), &mDxgiFactory);

#ifdef _DEBUG
		typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);

		HMODULE dxgidebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (dxgidebug)
		{
			auto dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(reinterpret_cast<void*>(GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));

			if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(mDxgiInfoQueue.GetAddressOf()))))
			{
				mDxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
				mDxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
			}
			else
			{
				HL_ERROR("Failed to get DXGIGetDebugInterface from dll\n");
			}
		}
		else
		{
			HL_ERROR("Failed to load dxgidebug.dll\n");
		}
#endif

	}

	Device::~Device()
	{

	}

}