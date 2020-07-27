#pragma once

#include "Drawable.h"

namespace Howlite {

	class HCube : public HDrawable {
	public:
		HCube(const std::string& Name, class HGraphicSystem& GraphicSystem);
		~HCube() = default;

		// Drawable interface
		DirectX::XMMATRIX GetTransform() const noexcept override;
		void Update(float DeltaTime) noexcept override;

	private:
		void OnResetPosition() noexcept;
		void OnResetRotation() noexcept;
		void OnResetColor() noexcept;

		struct ColorBuffer {
			ColorBuffer() :
				Red{ 0.8f }, Green{ 0.8f }, Blue{ 0.8f }, Alpha{ 1.0f }
			{}

			ColorBuffer(float R, float G, float B, float A) :
				Red{ R }, Green{ G }, Blue{ B }, Alpha{ A }
			{}

			ColorBuffer(ColorBuffer&&) = default;
			ColorBuffer(const ColorBuffer&) = default;

			ColorBuffer& operator=(ColorBuffer&&) = default;
			ColorBuffer& operator=(const ColorBuffer&) = default;

			float Red;
			float Green;
			float Blue;
			float Alpha;
		};

		ColorBuffer mColorBuffer;
		const std::string mName;
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
	};

}
