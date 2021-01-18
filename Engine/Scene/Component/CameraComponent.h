#pragma once

#include "Engine/ECS/Component.h"

namespace Howlite {

	class CameraComponent : public TComponent<CameraComponent> {
	public:
		HL_CLASS_DEFAULT(CameraComponent);

		enum class ProjectionType : uint8_t {
			Perspective,
			Orthographic
		};

		void SetPerspective(const float InFieldOfView, const float InAspectRatio, const float InNearPlane, const float InFarPlane);
		void SetOrthographic(const float InAspectRatio, const float InScale);

		void SetNearPlane(const float InNearPlane) noexcept;
		void SetFarPlane(const float InFarPlane) noexcept;
		void SetAspectRatio(const float InAspectRatio) noexcept;
		void SetFieldOfView(const float InFieldOfView) noexcept;
		void SetScale(const float InScale) noexcept;

		float GetNearPlane() const noexcept;
		float GetFarPlane() const noexcept;
		float GetAspectRatio() const noexcept;
		float GetFieldOfView() const noexcept;
		float GetScale() const noexcept;

		inline const ProjectionType GetProjectionType() const noexcept { return mProjectionType; }
		inline const DirectX::XMMATRIX& GetProjectionMatrix() const noexcept { return mProjectionMatrix; }
		// NOTE: Should be called after we explicetly set  
		// any of projection properties
		void RecalculateProjectionMatrix(const ProjectionType InProjectionType);
		
	private:
		ProjectionType mProjectionType = ProjectionType::Perspective;
		DirectX::XMMATRIX mProjectionMatrix;
		float mNearPlane = 0.0f;
		float mFarPlane = 0.0f;
		float mAspectRatio = 0.0f;
		float mFieldOfView = 0.0f;
		float mScale = 0.0f;
	};

}