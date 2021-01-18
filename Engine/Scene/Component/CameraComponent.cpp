#include "CameraComponent.h"

namespace Howlite {
	
	void CameraComponent::SetPerspective(const float InFieldOfView, const float InAspectRatio, const float InNearPlane, const float InFarPlane)
	{
		mFieldOfView = InFieldOfView;
		mAspectRatio = InAspectRatio;
		mNearPlane = InNearPlane;
		mFarPlane = InFarPlane;
		mProjectionType = ProjectionType::Perspective;
		RecalculateProjectionMatrix(mProjectionType);
	}
	
	void CameraComponent::SetOrthographic(const float InAspectRatio, const float InScale)
	{
		mAspectRatio = InAspectRatio;
		mScale = InScale;
		mProjectionType = ProjectionType::Orthographic;
		RecalculateProjectionMatrix(mProjectionType);
	}

	void CameraComponent::SetNearPlane(const float InNearPlane) noexcept
	{
		mNearPlane = InNearPlane; 
	}

	void CameraComponent::SetFarPlane(const float InFarPlane) noexcept
	{
		mFarPlane = InFarPlane; 
	}

	void CameraComponent::SetAspectRatio(const float InAspectRatio) noexcept
	{
		mAspectRatio = InAspectRatio; 
	}

	void CameraComponent::SetFieldOfView(const float InFieldOfView) noexcept
	{ 
		HL_ASSERT(mProjectionType == ProjectionType::Perspective, "Orthographic projection doesn't support field of view property");
		mFieldOfView = InFieldOfView; 
	}

	void CameraComponent::SetScale(const float InScale) noexcept
	{
		HL_ASSERT(mProjectionType == ProjectionType::Orthographic, "Perspective projection doesn't support scale property");
		mScale = InScale; 
	}

	float CameraComponent::GetNearPlane() const noexcept
	{
		return mNearPlane; 
	}

	float CameraComponent::GetFarPlane() const noexcept
	{
		return mFarPlane; 
	}

	float CameraComponent::GetAspectRatio() const noexcept
	{
		return mAspectRatio; 
	}

	float CameraComponent::GetFieldOfView() const noexcept
	{
		HL_ASSERT(mProjectionType == ProjectionType::Perspective, "Orthographic projection doesn't support field of view property");
		return mFieldOfView; 
	}

	float CameraComponent::GetScale() const noexcept
	{
		HL_ASSERT(mProjectionType == ProjectionType::Orthographic, "Perspective projection doesn't support scale property");
		return mScale; 
	}

	void CameraComponent::RecalculateProjectionMatrix(const ProjectionType InProjectionType)
	{
		switch (InProjectionType)
		{
			case ProjectionType::Perspective:
				mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
				break;
			case ProjectionType::Orthographic:
				HL_ASSERT(false, "case ProjectionType::Orthographic is not implemented");
				break;
		}
	}

}