#include "SwapChain.h"
#include "Device.h"

namespace Howlite {

	SwapChain::SwapChain(const Device* InDevice, HWND InHandle, const uint32_t InWidth, const uint32_t InHeight) :
		mDevice{ InDevice }
	{
		// create swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = InWidth;
		swapChainDesc.BufferDesc.Height = InHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = InHandle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		mDevice->GetDxgiFactory()->CreateSwapChain(mDevice->GetDevice(), &swapChainDesc, &mSwapChain);
		HL_ASSERT(mSwapChain, "Failed to create swapchain");

		// create render target
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
		mDevice->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &mRenderTargetView);
		HL_ASSERT(mRenderTargetView, "Failed to create render target view");

		// create depth stencil
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
		ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
		depthStencilTextureDesc.Width = InWidth;
		depthStencilTextureDesc.Height = InHeight;
		depthStencilTextureDesc.MipLevels = 1u;
		depthStencilTextureDesc.ArraySize = 1u;
		depthStencilTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		depthStencilTextureDesc.SampleDesc.Count = 1u;
		depthStencilTextureDesc.SampleDesc.Quality = 0u;
		depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		mDevice->GetDevice()->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilTexture);
		HL_ASSERT(depthStencilTexture, "Failed to create depth stencil texture");

		// create target view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		mDevice->GetDevice()->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilViewDesc, &mDepthStencilView);
		HL_ASSERT(mDepthStencilView, "Failed to create depth stencil view");
	}

	SwapChain::~SwapChain()
	{
		mDevice->GetDeviceContext()->OMSetRenderTargets(0u, nullptr, nullptr);
	}

	void SwapChain::ClearDepthStencilView()
	{
		mDevice->GetDeviceContext()->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}

	void SwapChain::ClearRenderTargetView(const std::array<float, 4>& InClearColor)
	{
		mDevice->GetDeviceContext()->ClearRenderTargetView(mRenderTargetView.Get(), InClearColor.data());
	}

	void SwapChain::Present(const bool IsVerticalSncEnabled)
	{
		mSwapChain->Present(IsVerticalSncEnabled ? 1u : 0u, 0u);
	}

	void SwapChain::Bind()
	{
		mDevice->GetDeviceContext()->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}

}