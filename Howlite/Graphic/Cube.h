#pragma once

#include "Drawable.h"

namespace Howlite {

	class HCube : public HDrawable {
	public:
		HCube(class HGraphicSystem& GraphicSystem);
		~HCube() override = default;

		// Drawable interface
		DirectX::XMMATRIX GetTransform() const noexcept override;

	private:
		void OnResetPosition() noexcept;
		void OnResetRotation() noexcept;

		struct ColorBuffer {
			alignas(16) DirectX::XMFLOAT3 Color;
			float SpecularIntensity;
			float SpecularPower;
			float padding[2];
		};

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
	};

}
