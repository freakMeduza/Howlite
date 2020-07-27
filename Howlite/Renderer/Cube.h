#pragma once

#include "Drawable.h"

namespace Howlite {

	class HCube : public HDrawable {
	public:
		HCube(class HGraphicSystem& GraphicSystem);
		~HCube() = default;

		// Drawable interface
		DirectX::XMMATRIX GetTransform() const noexcept override;
		void Update(float DeltaTime) noexcept override;

		/** 
		 * Spawn UI Window
		 */
		void SpwanUIWindow();

	private:
		void OnReset() noexcept;

		struct Color
		{
			float R{ 0.0f };
			float G{ 0.0f };
			float B{ 0.0f };
			float A{ 0.0f };
		};

		struct Transform
		{
			DirectX::XMMATRIX ModelMatrix;
			DirectX::XMMATRIX ViewMatrix;
			DirectX::XMMATRIX ProjectionMatrix;
		};

		Color mColor;
		Transform mTransform;

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
	};

}
