#pragma once

#include "Drawable.h"
#include "Common/Color.h"

namespace Howlite {

	class HSphere : public HDrawable {
	public:
		HSphere(HGraphicSystem& GraphicSystem, const DirectX::XMFLOAT3& Position, float Radius = 0.5f);
		~HSphere() override = default;

		// Drawable interface
		DirectX::XMMATRIX GetTransform() const noexcept override;

		/**
		 * Set Color
		 * @param Color 
		 * @return 
		 */
		void SetColor(const HColor& Color) noexcept;

		/**
		 * Get Color
		 * @return 
		 */
		HColor GetColor() const noexcept;

		/**
		 * Set Position
		 * @param Position 
		 * @return 
		 */
		void SetPosition(const DirectX::XMFLOAT3& Position) noexcept;

		/**
		 * Get Position
		 * @return 
		 */
		DirectX::XMFLOAT3 GetPosition() const noexcept;

	private:
		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT4 mColor;

	};

}
