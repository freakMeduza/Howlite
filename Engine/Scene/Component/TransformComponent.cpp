#include "TransformComponent.h"

namespace Howlite {

	void TransformComponent::SetTranslation(const DirectX::XMFLOAT3& InTranslation)
	{
		mTranslation = InTranslation;
	}

	void TransformComponent::SetScale(const DirectX::XMFLOAT3& InScale)
	{
		mScale = InScale;
	}

	void TransformComponent::SetRotation(const DirectX::XMVECTOR& InRotation)
	{
		mRotation = InRotation;
	}

	const DirectX::XMFLOAT3& TransformComponent::GetTranslation() const noexcept
	{
		return mTranslation;
	}

	const DirectX::XMFLOAT3& TransformComponent::GetScale() const noexcept
	{
		return mScale;
	}

	const DirectX::XMVECTOR& TransformComponent::GetRotation() const noexcept
	{
		return mRotation;
	}

	void TransformComponent::Translate(const DirectX::XMFLOAT3& InDistance)
	{
		mTranslation.x += InDistance.x;
		mTranslation.y += InDistance.y;
		mTranslation.z += InDistance.z;
	}

	void TransformComponent::TranslateX(const float InDistance)
	{
		Translate({ InDistance, 0.0f, 0.0f });
	}

	void TransformComponent::TranslateY(const float InDistance)
	{
		Translate({ 0.0f, InDistance, 0.0f });
	}

	void TransformComponent::TranslateZ(const float InDistance)
	{
		Translate({ 0.0f, 0.0f, InDistance });
	}

	void TransformComponent::Scale(const DirectX::XMFLOAT3& InScaleFactor)
	{
		mScale.x *= InScaleFactor.x;
		mScale.y *= InScaleFactor.y;
		mScale.z *= InScaleFactor.z;
	}

	void TransformComponent::Scale(const float InScaleFactor)
	{
		Scale(DirectX::XMFLOAT3{ InScaleFactor, InScaleFactor, InScaleFactor });
	}

	void TransformComponent::ScaleX(const float InScaleFactor)
	{
		Scale(DirectX::XMFLOAT3{ InScaleFactor, 1.0f, 1.0f });
	}

	void TransformComponent::ScaleY(const float InScaleFactor)
	{
		Scale(DirectX::XMFLOAT3{ 1.0f, InScaleFactor, 1.0f });
	}

	void TransformComponent::ScaleZ(const float InScaleFactor)
	{
		Scale(DirectX::XMFLOAT3{ 1.0f, 1.0f, InScaleFactor });
	}

	void TransformComponent::Rotate(const DirectX::XMVECTOR& InRotationFactor)
	{
		using namespace DirectX;
		mRotation *= InRotationFactor;
	}

	void TransformComponent::Rotate(const float InAngle, const DirectX::XMFLOAT3& InAxis)
	{
		Rotate(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(InAxis.x, InAxis.y, InAxis.z, 0.0f), DirectX::XMConvertToRadians(InAngle)));
	}

	void TransformComponent::RotateX(const float InAngle)
	{
		Rotate(InAngle, { 1.0f, 0.0f, 0.0f });
	}

	void TransformComponent::RotateY(const float InAngle)
	{
		Rotate(InAngle, { 0.0f, 1.0f, 0.0f });
	}

	void TransformComponent::RotateZ(const float InAngle)
	{
		Rotate(InAngle, { 0.0f, 0.0f, 1.0f });
	}

}