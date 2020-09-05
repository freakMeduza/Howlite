#pragma once

#include "ConstantBuffer.h"

namespace Howlite {

	class HCamera {
	public:
		HCamera(HGraphicSystem& GraphicSystem, const DirectX::XMMATRIX& ProjectionMatrix);
		virtual ~HCamera() = default;

		/**
		 * Translate Camera
		 * @param Delta 
		 */
		void Translate(const float Delta);

		/**
		 * Rotate Camera
		 * @param Pitch 
		 * @param Yaw 
		 * @return 
		 */
		void Rotate(const float Pitch, const float Yaw) noexcept;

		/**
		 * Set Projection Matrix
		 * @param ProjectionMatrix 
		 * @return 
		 */
		void SetProjectionMatrix(const DirectX::XMMATRIX& ProjectionMatrix) noexcept;

		/**
		 * Get Projection Matrix
		 * @return 
		 */
		const DirectX::XMMATRIX GetProjectionMatrix() const noexcept;

		/**
		 * Get View Matrix
		 * @return 
		 */
		const DirectX::XMMATRIX GetViewMatrix() const noexcept;

		/**
		 * Get Position
		 * @return 
		 */
		const DirectX::XMFLOAT3 GetPosition() const noexcept;

		/**
		 * Bind Camera Object
		 * @param GraphicSystem 
		 */
		void Bind(HGraphicSystem& GraphicSystem) const;

		/**
		 * Draw UI Window
		 * @param IsOpen
		 * @return
		 */
		void DrawUIWindow(bool* IsOpen) noexcept;

	protected:
		void OnReset() noexcept;
		void OnTransform() noexcept;

		DirectX::XMMATRIX mProjectionMatrix;

		mutable DirectX::XMFLOAT3 mPosition;

		float mRoll;
		float mPitch;
		float mYaw;
		float mRadius;
		float mSensitivity;
		float mSpeed;

		mutable SharedPointer<HPixelConstantBuffer<HCameraData>> mCameraBuffer{ nullptr };

	};

}
