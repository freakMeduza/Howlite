#pragma once

#include "ConstantBuffer.h"
#include "UI/UISystem.h"

namespace Howlite {

	class HLight {
	public:
		HLight(HGraphicSystem& GraphicSystem, float Radius = 0.5f);
		~HLight();

		HLight(HLight&&) = delete;
		HLight(const HLight&) = delete;

		HLight& operator=(HLight&&) = delete;
		HLight& operator=(const HLight&) = delete;

		/**
		 * Draw Light Object
		 * @param GraphicSystem 
		 */
		void Draw(HGraphicSystem& GraphicSystem) const;

		/**
		 * Bind Light Object
		 * @param GraphicSystem 
		 */
		void Bind(HGraphicSystem& GraphicSystem) const;

	private:
		void OnReset() noexcept;

		struct LightBuffer
		{
			alignas(16) DirectX::XMFLOAT3 Position;
			alignas(16) DirectX::XMFLOAT3 Color;
			float Intencity;
			float Kc;
			float Kl;
			float Kq;
		} mBuffer;

		mutable ScopedPointer<class HSphere> mMesh{ nullptr };
		mutable ScopedPointer<HPixelConstantBuffer<LightBuffer>> mLightBuffer{ nullptr };

		HUIComponent mUIComponent;
	};

}
