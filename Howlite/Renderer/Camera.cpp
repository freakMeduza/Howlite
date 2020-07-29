#include "HowlitePCH.h"
#include "Camera.h"
#include "Common/Math.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Input/InputSystem.h"
#include "Renderer/UI/UISystem.h"

namespace Howlite {

	HCamera::HCamera(const DirectX::XMMATRIX& ProjectionMatrix) :
		mProjectionMatrix{ ProjectionMatrix }, mRoll{ 0.0f }, mPitch{ 0.0f }, mYaw{ 0.0f }, mRadius{ 10.0f }, mSensitivity{ 0.015f }, mSpeed{ 0.2f }
	{
		HEngine::GetInstance().GetUISystemInstance().BindUIComponent(HUISystem::CreateUIComponent([this]()
		{
			if (ImGui::Begin("Camera", (bool*)nullptr))
			{
				ImGui::Text("Position:");
				ImGui::SliderFloat("radius", &mRadius, 3.0f, 90.0f, "%.1f");
				ImGui::SliderAngle("pitch", &mPitch, -89.0f, 89.0f);
				ImGui::SliderAngle("yaw", &mYaw, -180.0f, 180.0f);
				ImGui::Text("Settings");
				ImGui::InputFloat("sensitivity", &mSensitivity, 0.0f, 0.0f, "%.3f");
				ImGui::InputFloat("speed", &mSpeed, 0.0f, 0.0f, "%.1f");
				if(ImGui::Button("Reset"))
				{
					OnReset();
				}
				ImGui::End();
			}
		}));
	}

	void HCamera::Translate(const float Delta)
	{
		mRadius -= mSpeed * Delta;
		mRadius = std::clamp(mRadius, 3.0f, 90.0f);
	}

	void HCamera::Rotate(const float Pitch, const float Yaw) noexcept
	{
		mPitch += mSensitivity * Pitch;
		mYaw += mSensitivity * Yaw;
		mPitch = std::clamp(mPitch, -H_PI * 0.995f / 2.0f, H_PI * 0.995f / 2.0f);
		mYaw = std::clamp(mYaw, -H_PI * 0.995f / 2.0f, H_PI * 0.995f / 2.0f);
	}

	const DirectX::XMMATRIX HCamera::GetProjectionMatrix() const noexcept
	{
		return mProjectionMatrix;
	}

	const DirectX::XMMATRIX HCamera::GetViewMatrix() const noexcept
	{
		using namespace DirectX;
		const XMVECTOR& position = XMVector3Transform(XMVectorSet(0.0f, 0.0f, -mRadius, 0.0f), XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0.0f));
		const XMMATRIX& viewMatrix = XMMatrixLookAtLH(position, XMVectorZero(), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0));
		return viewMatrix;
	}

	void HCamera::OnReset() noexcept
	{
		mRoll = 0.0f;
		mPitch = 0.0f;
		mYaw = 0.0f;
		mRadius = 10.0f;
		mSensitivity = 0.015f;
		mSpeed = 0.2f;
	}

}