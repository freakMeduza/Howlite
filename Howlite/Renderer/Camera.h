#pragma once

namespace Howlite {

	class HCamera {
	public:
		HCamera(const DirectX::XMMATRIX& ProjectionMatrix);
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
		 * Get Projection Matrix
		 * @return 
		 */
		const DirectX::XMMATRIX GetProjectionMatrix() const noexcept;

		/**
		 * Get View Matrix
		 * @return 
		 */
		const DirectX::XMMATRIX GetViewMatrix() const noexcept;

	protected:
		void OnReset() noexcept;

		DirectX::XMMATRIX mProjectionMatrix;
		DirectX::XMMATRIX mViewMatrix;

		float mRoll;
		float mPitch;
		float mYaw;
		float mRadius;
		float mSensitivity;
		float mSpeed;

	};

}
