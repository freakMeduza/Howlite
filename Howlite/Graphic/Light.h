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
		
		/**
		 * Draw UI Window
		 * @param IsOpen 
		 * @return 
		 */
		void DrawUIWindow(bool* IsOpen) noexcept;

	private:
		void OnReset() noexcept;

		HLightData mLightData;
		mutable ScopedPointer<class HSphere> mMesh{ nullptr };
		mutable ScopedPointer<HPixelConstantBuffer<HLightData>> mLightBuffer{ nullptr };

	};

}
