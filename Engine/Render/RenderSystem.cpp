#include "RenderSystem.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Engine/Event/EventManager.h"
#include "Device.h"
#include "SwapChain.h"

namespace Howlite {

	RenderSystem::RenderSystem() : TSystem{ Priority::Hight }
	{
		mDevice = std::make_unique<Device>();

		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Subscribe<WindowCreateEvent>("RenderSystem::OnWindowCreated", [this](const WindowCreateEvent* InEvent) { OnWindowCreated(InEvent); });
		eventManager->Subscribe<WindowDestroyEvent>("RenderSystem::OnWindowDestroyed", [this](const WindowDestroyEvent* InEvent) { OnWindowDestroyed(InEvent); });
	}
	
	RenderSystem::~RenderSystem()
	{
		EventManager* eventManager = Engine::GetInstance()->GetEventManager();
		eventManager->Unsubscribe<WindowCreateEvent>("RenderSystem::OnWindowCreated");
		eventManager->Unsubscribe<WindowDestroyEvent>("RenderSystem::OnWindowDestroyed");
	}

	void RenderSystem::BeginFrame(HWND InWindowHandle, D3D11_VIEWPORT InViewport, const std::array<float, 4>& InClearColor)
	{
		HL_ASSERT(!mIsFrame, "Begin frame called twice before end frame");
		auto it = mSwapChainRegistry.find(InWindowHandle);
		HL_ASSERT(it != mSwapChainRegistry.end(), "Swap chain registry corrupted, can't find swap chain for window");
		mTargetSwapChain = it->second.get();
		mDevice->GetDeviceContext()->RSSetViewports(1u, &InViewport);
		mTargetSwapChain->Bind();
		mTargetSwapChain->ClearDepthStencilView();
		mTargetSwapChain->ClearRenderTargetView(InClearColor);
		mIsFrame = true;
	}

	void RenderSystem::EndFrame(const bool IsVerticalSyncEnabled)
	{
		HL_ASSERT(mIsFrame, "End frame called twice after begin frame");
		HL_ASSERT(mTargetSwapChain, "Target swap chain doesn't specified");
		mTargetSwapChain->Present(IsVerticalSyncEnabled);
		mIsFrame = false;
	}

	void RenderSystem::DrawIndexed(const size_t InIndexCount)
	{
		mDevice->GetDeviceContext()->DrawIndexed((UINT)InIndexCount, 0u, 0u);
	}

	void RenderSystem::OnWindowCreated(const WindowCreateEvent* InEvent)
	{
		auto swapchain = std::make_unique<SwapChain>(mDevice.get(), InEvent->mHandle, InEvent->mWidth, InEvent->mHeight);
		HL_ASSERT(swapchain, "Failed to create swap chain for window");
		mSwapChainRegistry.insert({ InEvent->mHandle, std::move(swapchain) });
	}

	void RenderSystem::OnWindowDestroyed(const WindowDestroyEvent* InEvent)
	{
		if (auto it = mSwapChainRegistry.find(InEvent->mHandle); it != mSwapChainRegistry.end())
		{
			it->second.reset(nullptr);
			mSwapChainRegistry.erase(it);
		}
	}

	ID3DBlob* RenderSystem::CompileShader(const wchar_t* InFileName, const char* InEntryPoint, const char* InProfile)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		HRESULT hr = ::D3DCompileFromFile(
			InFileName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			InEntryPoint,
			InProfile,
			flags,
			0,
			&shaderBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				HL_ERROR("%s", (char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
			{
				shaderBlob->Release();
			}

			return nullptr;
		}


		return shaderBlob;
	}

}