#pragma once

namespace Howlite {

	struct HColor {
		constexpr HColor(float R, float G, float B, float A)
		{
			mData[0] = R;
			mData[1] = G;
			mData[2] = B;
			mData[3] = A;
		}

		inline const float* GetData() const noexcept
		{
			return mData.data();
		}

		inline float GetR() const noexcept
		{
			return mData[0];
		}

		inline float GetG() const noexcept
		{
			return mData[1];
		}

		inline float GetB() const noexcept
		{
			return mData[2];
		}

		inline float GetA() const noexcept
		{
			return mData[3];
		}

		inline static constexpr HColor Red() noexcept
		{
			return { 1.0f, 0.0f, 0.0f, 1.0f };
		}

		inline static constexpr HColor Blue() noexcept
		{
			return { 0.0f, 0.0f, 1.0f, 1.0f };
		}

		inline static constexpr HColor Green() noexcept
		{
			return { 0.0f, 1.0f, 0.0f, 1.0f };
		}

		inline static constexpr HColor White() noexcept
		{
			return { 1.0f, 1.0f, 1.0f, 1.0f };
		}

		inline static constexpr HColor Black() noexcept
		{
			return { 0.1f, 0.1f, 0.1f, 1.0f };
		}

		inline static constexpr HColor Fuxia() noexcept
		{
			return { 1.0f, 0.0f, 1.0f, 1.0f };
		}

	private:
		std::array<float, 4> mData;

	};

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
		 */
		void EndFrame();

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
		 * Set VSync Is Enabled
		 * @param VSyncEnabled 
		 * @return 
		 */
		void SetVSyncEnabled(bool VSyncEnabled) noexcept;

		/**
		 * Is VSync Is Enabled
		 * @return 
		 */
		bool IsVSyncEnabled() const noexcept;

		/** 
		 * Get UI System Instance
		 */
		class HUISystem& GetUISystemInstance();

	private:
		friend class HBindable;

		void CreateDeviceAndSwapchain(HWND hWnd, uint32_t Width, uint32_t Height);
		void CreateViewport(uint32_t Width, uint32_t Height);
		void CreateRenderTargetView();

		Microsoft::WRL::ComPtr<ID3D11Device> mDevice{ nullptr };
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapchain{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext{ nullptr };

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView{ nullptr };

		ScopedPointer<class HUISystem> mUISystem{ nullptr };
		ScopedPointer<class HDXGIInfoQueue> mDXGIInfoQueue{ nullptr };

		bool mVSyncIsEnabled{ false };
	};

}
