#include "HowlitePCH.h"
#include "GraphicSystem.h"
#include "DXGIInfoQueue.h"
#include "UI/UISystem.h"
#include "Engine/Window.h"
#include "Event/WindowEvent.h"

namespace Howlite {

	HGraphicSystem::HGraphicSystem(HWND hWnd, uint32_t Width, uint32_t Height)
	{
		CreateDeviceAndSwapchain(hWnd, Width, Height);
		CreateViewport(Width, Height);
		CreateRenderTargetView();

		mUISystem = CreateScopedPointer<HUISystem>(hWnd, mDevice.Get(), mContext.Get());
		mUISystem->SetIsEnabled(true);
	
#if _DEBUG
		mDXGIInfoQueue = CreateScopedPointer<HDXGIInfoQueue>();
#endif
	}

	HGraphicSystem::~HGraphicSystem()
	{
		
	}

	void HGraphicSystem::BeginFrame(HColor Color)
	{
		if (GetUISystemInstance().IsEnabled())
		{
			GetUISystemInstance().BeginFrame();
		}

		mContext->ClearRenderTargetView(mRenderTargetView.Get(), Color.GetData());
		mContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), nullptr);
	}

	void HGraphicSystem::EndFrame()
	{
		if (GetUISystemInstance().IsEnabled())
		{
			GetUISystemInstance().EndFrame();
		}

		mSwapchain->Present(mVSyncIsEnabled ? 1u : 0u, 0u);
	}

	void HGraphicSystem::DrawIndexed(UINT IndexCount)
	{
	#if _DEBUG
		mDXGIInfoQueue->ClearMessageQueue();
		mContext->DrawIndexed(IndexCount, 0u, 0u);
		const std::vector<std::string>& messages = mDXGIInfoQueue->GetMessageQueue();
		if(!messages.empty())
		{
			std::string errorString;
			for(const std::string& line : messages)
			{
				errorString += line;
				errorString.push_back('\n');
			}

			if (!errorString.empty())
			{
				errorString.pop_back();
			}

			throw HException{ __FILE__, __LINE__, errorString };
		}
	#else
		mContext->DrawIndexed(IndexCount, 0u, 0u);
	#endif
	}

	void HGraphicSystem::ResizeBuffers(uint32_t Width, uint32_t Height)
	{
		mRenderTargetView.Reset();
		mSwapchain->ResizeBuffers(1u, Width, Height, DXGI_FORMAT_UNKNOWN, 0u);
		CreateRenderTargetView();
		CreateViewport(Width, Height);
	}

	void HGraphicSystem::SetVSyncEnabled(bool VSyncEnabled) noexcept
	{
		mVSyncIsEnabled = VSyncEnabled;
	}

	bool HGraphicSystem::IsVSyncEnabled() const noexcept
	{
		return mVSyncIsEnabled;
	}

	HUISystem& HGraphicSystem::GetUISystemInstance()
	{
		H_ASSERT(mUISystem != nullptr, "Failed to get UI system instance 'mUISystem == nullptr'.")
		return *mUISystem;
	}

	void HGraphicSystem::CreateDeviceAndSwapchain(HWND hWnd, uint32_t Width, uint32_t Height)
	{
		DXGI_SWAP_CHAIN_DESC swapchain;

		ZeroMemory(&swapchain, sizeof(swapchain));

		swapchain.BufferDesc.Width                   = Width;
		swapchain.BufferDesc.Height                  = Height;
		swapchain.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
		swapchain.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapchain.BufferDesc.RefreshRate.Numerator   = 60;
		swapchain.BufferDesc.RefreshRate.Denominator = 1;
		swapchain.SampleDesc.Count                   = 1;
		swapchain.SampleDesc.Quality                 = 0;
		swapchain.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain.BufferCount                        = 1;
		swapchain.OutputWindow                       = hWnd;
		swapchain.Windowed                           = TRUE;
		swapchain.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
		swapchain.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		UINT createDeviceFlags = 0u;
		
		D3D_FEATURE_LEVEL featureLevel;

		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	#ifndef NDEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		{
			HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr,
															D3D_DRIVER_TYPE_HARDWARE,
															nullptr,
															createDeviceFlags,
															featureLevelArray,
															1,
															D3D11_SDK_VERSION,
															&swapchain,
															&mSwapchain,
															&mDevice,
															&featureLevel,
															&mContext);

			if(FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}
	}

	void HGraphicSystem::CreateRenderTargetView()
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> buffer{ nullptr };
		
		{
			HRESULT result = mSwapchain->GetBuffer(0, __uuidof(ID3D11Resource), &buffer);
			if(FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}

		{
			HRESULT result = mDevice->CreateRenderTargetView(buffer.Get(), nullptr, &mRenderTargetView);
			if (FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}
	}

	void HGraphicSystem::CreateViewport(uint32_t Width, uint32_t Height)
	{
		D3D11_VIEWPORT viewport;
		viewport.Width    = static_cast<float>(Width);
		viewport.Height   = static_cast<float>(Height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		mContext->RSSetViewports(1u, &viewport);
	}

}
