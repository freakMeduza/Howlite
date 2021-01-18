#pragma once

#include "Engine/ECS/System.h"

namespace Howlite {

	class Device;
	class SwapChain;
	class VertexBuffer;
	class IndexBuffer;
	class InputLayout;
	class VertexShader;
	class PixelShader;
	class Topology;

	class RenderSystem final : public TSystem<RenderSystem>	{
	public:
		HL_DISABLE_COPY(RenderSystem);

		RenderSystem();
		~RenderSystem();
		
		void BeginFrame(HWND InWindowHandle, D3D11_VIEWPORT InViewport, const std::array<float, 4>& InClearColor);
		void EndFrame(const bool IsVerticalSyncEnabled);
		void DrawIndexed(const size_t InIndexCount);

		inline Device* GetDevice() const noexcept { return mDevice.get(); }

		SwapChain* GetSwapChain(HWND InWindowHandle) const;

		ID3DBlob* ReadShader(const wchar_t* InFileName);
		ID3DBlob* CompileShader(const wchar_t* InFileName, const char* InEntryPoint, const char* InProfile);

	private:
		void OnWindowCreated(const class WindowCreateEvent* InEvent);
		void OnWindowDestroyed(const class WindowDestroyEvent* InEvent);
		
		bool mIsFrame = false;
		SwapChain* mTargetSwapChain = nullptr;
		std::unique_ptr<Device> mDevice = nullptr;
		std::unordered_map<HWND, std::unique_ptr<SwapChain>> mSwapChainRegistry;

	};

}