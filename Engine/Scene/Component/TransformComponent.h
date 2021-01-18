#pragma once

#include "Engine/ECS/Component.h"

namespace Howlite {

	class TransformComponent : public TComponent<TransformComponent> {
	public:
		HL_CLASS_DEFAULT(TransformComponent);

		void SetTranslation(const DirectX::XMFLOAT3& InTranslation);
		void SetScale(const DirectX::XMFLOAT3& InScale);
		void SetRotation(const DirectX::XMVECTOR& InRotation);

		const DirectX::XMFLOAT3& GetTranslation() const noexcept;
		const DirectX::XMFLOAT3& GetScale() const noexcept;
		const DirectX::XMVECTOR& GetRotation() const noexcept;

		void Translate(const DirectX::XMFLOAT3& InDistance);
		void TranslateX(const float InDistance);
		void TranslateY(const float InDistance);
		void TranslateZ(const float InDistance);

		void Scale(const DirectX::XMFLOAT3& InScaleFactor);
		void Scale(const float InScaleFactor);
		void ScaleX(const float InScaleFactor);
		void ScaleY(const float InScaleFactor);
		void ScaleZ(const float InScaleFactor);

		void Rotate(const DirectX::XMVECTOR& InRotationFactor);
		void Rotate(const float InAngle, const DirectX::XMFLOAT3& InAxis);
		void RotateX(const float InAngle);
		void RotateY(const float InAngle);
		void RotateZ(const float InAngle);

	private:
		DirectX::XMMATRIX mTransform;
		DirectX::XMVECTOR mRotation;
		DirectX::XMFLOAT3 mTranslation;
		DirectX::XMFLOAT3 mScale;
	};

}