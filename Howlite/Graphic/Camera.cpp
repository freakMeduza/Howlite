#include "HowlitePCH.h"
#include "Camera.h"
#include "Common/Math.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Input/InputSystem.h"
#include "UI/UISystem.h"

namespace Howlite {

	HCamera::HCamera(const DirectX::XMMATRIX& ProjectionMatrix) :
		mProjectionMatrix{ ProjectionMatrix }
	{
		OnReset();
	}

	void HCamera::Translate(const float Delta)
	{
		mRadius -= mSpeed * Delta;
		mRadius = std::clamp(mRadius, 10.0f, 1000.0f);
		OnTransform();
	}

	void HCamera::Rotate(const float Pitch, const float Yaw) noexcept
	{
		mPitch += mSensitivity * Pitch;
		mYaw += mSensitivity * Yaw;
		mPitch = std::clamp(mPitch, -H_PI * 0.995f / 2.0f, H_PI * 0.995f / 2.0f);
		mYaw = std::clamp(mYaw, -H_2PI * 0.995f / 2.0f, H_2PI * 0.995f / 2.0f);
		OnTransform();
	}

	void HCamera::SetProjectionMatrix(const DirectX::XMMATRIX& ProjectionMatrix) noexcept
	{
		mProjectionMatrix = ProjectionMatrix;
	}

	const DirectX::XMMATRIX HCamera::GetProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	const DirectX::XMMATRIX HCamera::GetViewMatrix() const noexcept
	{
		using namespace DirectX;
		const XMMATRIX& viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMVectorZero(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0));
		return viewMatrix;
	}

	const DirectX::XMFLOAT3 HCamera::GetPosition() const noexcept
	{
		return mPosition;
	}

	void HCamera::DrawUIWindow(bool* IsOpen) noexcept
	{
		const ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

		if (!ImGui::Begin("Camera", IsOpen, flags))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Position");
		ImGui::SliderFloat("radius", &mRadius, 10.0f, 1000.0f, "%.1f");
		ImGui::SliderAngle("pitch", &mPitch, -90.0f * 0.995f, 90.0f * 0.995f, "%.1f");
		ImGui::SliderAngle("yaw", &mYaw, -180.0f * 0.995f, 180.0f * 0.995f, "%.1f");
		ImGui::Separator();
		ImGui::Text("Settings");
		ImGui::InputFloat("Sensitivity", &mSensitivity, 0.0f, 0.0f, "%.3f");
		ImGui::InputFloat("Speed", &mSpeed, 0.0f, 0.0f, "%.1f");
		if (ImGui::Button("Reset"))
		{
			OnReset();
		}
		ImGui::Separator();
		ImGui::End();

		OnTransform();
	}

	void HCamera::OnReset() noexcept
	{
		mRoll        = 0.0f;
		mPitch       = 0.0f;
		mYaw         = 0.0f;
		mRadius      = 500.0f;
		mPosition    = { 0.0f, 0.0f, -mRadius };
		mSensitivity = 0.008f;
		mSpeed       = 1.0f;
	}

	void HCamera::OnTransform() noexcept
	{
		using namespace DirectX;
		XMStoreFloat3(&mPosition, XMVector3Transform(XMVectorSet(0.0f, 0.0f, -mRadius, 0.0f), XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0.0f)));
	}

}