#include "HowlitePCH.h"
#include "GraphicSystem.h"
#include "DXGIInfoQueue.h"
#include "UI/UISystem.h"
#include "Engine/Window.h"
#include "Event/WindowEvent.h"

namespace Howlite {

	HGraphicSystem::HGraphicSystem(HWND hWnd, uint32_t Width, uint32_t Height)
	{
		// create device and swapchain
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

			if (FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}

		// create depth stencil state
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

		D3D11_DEPTH_STENCIL_DESC depthStencil;

		ZeroMemory(&depthStencil, sizeof(depthStencil));

		depthStencil.DepthEnable = TRUE;
		depthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencil.DepthFunc = D3D11_COMPARISON_LESS;

		{
			HRESULT result = mDevice->CreateDepthStencilState(&depthStencil, &depthStencilState);
			if (FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}

		mContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);

		InitializeDepthStencilView(Width, Height);
		InitializeRenderTargetView();
		InitializeViewport(Width, Height);

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

		const float color[] = {
			Color.GetRed<float>(),
			Color.GetGreen<float>(),
			Color.GetBlue<float>(),
			Color.GetAlpha<float>()
		};

		mContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void HGraphicSystem::EndFrame(bool VSync)
	{
		if (GetUISystemInstance().IsEnabled())
		{
			GetUISystemInstance().Draw();
			GetUISystemInstance().EndFrame();
		}

		mSwapchain->Present(VSync? 1u : 0u, 0u);
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
		mDepthStencilView.Reset();
		mSwapchain->ResizeBuffers(1u, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0u);
		InitializeRenderTargetView();
		InitializeDepthStencilView(Width, Height);
		InitializeViewport(Width, Height);
	}

	HUISystem& HGraphicSystem::GetUISystemInstance()
	{
		H_ASSERT(mUISystem != nullptr, "Failed to get UI system instance 'mUISystem == nullptr'.")
		return *mUISystem;
	}

	void HGraphicSystem::InitializeDepthStencilView(uint32_t Width, uint32_t Height)
	{
		// create depth stencil texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;
		
		D3D11_TEXTURE2D_DESC texture;;

		ZeroMemory(&texture, sizeof(texture));

		texture.Width              = Width;
		texture.Height             = Height;
		texture.MipLevels          = 1u;
		texture.ArraySize          = 1u;
		texture.Format             = DXGI_FORMAT_D32_FLOAT;
		texture.SampleDesc.Count   = 1u;
		texture.SampleDesc.Quality = 0u;
		texture.Usage              = D3D11_USAGE_DEFAULT;
		texture.BindFlags          = D3D11_BIND_DEPTH_STENCIL;

		{
			HRESULT result = mDevice->CreateTexture2D(&texture, nullptr, &buffer);
			if(FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}

		// create view of depth stencil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilView;

		ZeroMemory(&depthStencilView, sizeof(depthStencilView));

		depthStencilView.Format             = DXGI_FORMAT_D32_FLOAT;
		depthStencilView.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilView.Texture2D.MipSlice = 0u;
		
		{
			HRESULT result = mDevice->CreateDepthStencilView(buffer.Get(), &depthStencilView, &mDepthStencilView);
			if(FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}
	}

	void HGraphicSystem::InitializeRenderTargetView()
	{
		// get back buffer texture
		Microsoft::WRL::ComPtr<ID3D11Resource> buffer{ nullptr };
		{
			HRESULT result = mSwapchain->GetBuffer(0, __uuidof(ID3D11Resource), &buffer);
			if(FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}
		// create view of back buffer texture
		{
			HRESULT result = mDevice->CreateRenderTargetView(buffer.Get(), nullptr, &mRenderTargetView);
			if (FAILED(result))
			{
				throw HException{ __FILE__, __LINE__, result, EExceptionType::Graphic };
			}
		}
	}

	void HGraphicSystem::InitializeViewport(uint32_t Width, uint32_t Height)
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
