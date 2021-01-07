#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	class Camera {
	public:
		HL_INTERFACE_DISABLE_COPY(Camera);

		inline void SetNearPlane(const float InNearPlane) noexcept { mNearPlane = InNearPlane; }
		inline void SetFarPlane(const float InFarPlane) noexcept { mFarPlane = InFarPlane; }
		inline void SetAspectRatio(const float InAspectRatio) noexcept { mAspectRatio = InAspectRatio; }
		inline void SetPosition(const DirectX::XMFLOAT3& InPosition) noexcept { mPosition = InPosition; }
		inline void SetRotation(const DirectX::XMVECTOR& InRotation) noexcept { mRotation = InRotation; }
		
		inline float GetNearPlane() const noexcept { return mNearPlane; }
		inline float GetFarPlane() const noexcept { return mFarPlane; }
		inline float GetAspectRatio() const noexcept { return mAspectRatio; }
		inline const DirectX::XMFLOAT3& GetPosition() const noexcept { return mPosition; }
		inline const DirectX::XMVECTOR& GetRotation() const noexcept { return mRotation; }

		// Matrix will be recalculated
		[[nodiscard]] const DirectX::XMMATRIX& GetViewMatrix() noexcept;
		[[nodiscard]] const DirectX::XMMATRIX& GetProjectionMatrix() noexcept;

	protected:
		virtual void RecalculateViewMatrix() = 0;
		virtual void RecalculateProjectionMatrix() = 0;

		DirectX::XMVECTOR GetOrientation() const noexcept;

		DirectX::XMMATRIX mViewMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX mProjectionMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMVECTOR mRotation = DirectX::XMVectorZero();
		DirectX::XMFLOAT3 mPosition = { 0.0f, 0.0f, 0.0f };

		float mNearPlane = 0.0f;
		float mFarPlane = 0.0f;
		float mAspectRatio = 0.0f;

	};

	class OrthographicCamera : public Camera {
	public:
		HL_CLASS_DISABLE_COPY(OrthographicCamera);

		inline void SetWidth(const float InWidth) noexcept { mWidth = InWidth; }
		inline void SetHeight(const float InHeight) noexcept { mHeight = InHeight; }

		inline float GetWidth() const noexcept { return mWidth; }
		inline float GetHeight() const noexcept { return mHeight; }

	protected:
		// Inherited via Camera
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

	private:
		float mWidth = 0.0f;
		float mHeight = 0.0f;

	};

	class PerspectiveCamera : public Camera {
	public:
		HL_CLASS_DISABLE_COPY(PerspectiveCamera);

		inline void SetFieldOfView(const float InFieldOfView) noexcept { mFieldOfView = InFieldOfView; }
		inline float GetFieldOfView() const noexcept { return mFieldOfView; }

	protected:
		// Inherited via Camera
		virtual void RecalculateViewMatrix() override;
		virtual void RecalculateProjectionMatrix() override;

	private:
		float mFieldOfView = 75.0f;

	};

}