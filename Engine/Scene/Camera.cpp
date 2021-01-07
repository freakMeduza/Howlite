#include "Camera.h"

namespace Howlite {

	const DirectX::XMMATRIX& Camera::GetViewMatrix() noexcept
	{
		RecalculateViewMatrix();
		return mViewMatrix;
	}

	const DirectX::XMMATRIX& Camera::GetProjectionMatrix() noexcept
	{
		RecalculateProjectionMatrix();
		return mProjectionMatrix;
	}

	DirectX::XMVECTOR Camera::GetOrientation() const noexcept
	{
		using namespace DirectX;
		return XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMMatrixRotationQuaternion(mRotation));
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HL_ASSERT(false, "void OrthographicCamera::RecalculateViewMatrix() is not implemented");
	}

	void OrthographicCamera::RecalculateProjectionMatrix()
	{
		mProjectionMatrix = DirectX::XMMatrixOrthographicLH(mWidth, mHeight, mNearPlane, mFarPlane);
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		using namespace DirectX;
		const auto& orientation = GetOrientation();
		const auto& position = XMLoadFloat3(&mPosition);
		const auto& target = position + orientation;
		mViewMatrix = XMMatrixLookAtLH(position, target, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(mFieldOfView, mAspectRatio, mNearPlane, mFarPlane);
	}

}