#include "HowlitePCH.h"
#include "Light.h"
#include "Sphere.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"

namespace {
	static const char* TAG = "Point Light";
}

namespace Howlite {

	HLight::HLight(HGraphicSystem& GraphicSystem, float Radius)
	{
		OnReset();
		mMesh = CreateScopedPointer<HSphere>(GraphicSystem, mBuffer.Position, Radius);
		mLightBuffer = CreateScopedPointer<HPixelConstantBuffer<LightBuffer>>(GraphicSystem, mBuffer, 1u);

		auto size = sizeof(LightBuffer);
		mUIComponent = HUISystem::CreateUIComponent([this]()
		{
			if(ImGui::Begin(TAG, (bool*)nullptr))
			{
				ImGui::DragFloat3("Position", (float*)&mBuffer.Position, 1.0f, 0.0f, 0.0f, "%.1f");
				ImGui::Separator();
				ImGui::ColorEdit3("Color", (float*)&mBuffer.Color);
				ImGui::Separator();
				ImGui::InputFloat("Intencity", &mBuffer.Intencity, 0.0f, 0.5f, "%.3f");
				ImGui::InputFloat("Constant", &mBuffer.Kc, 0.0f, 0.5f, "%.3f");
				ImGui::InputFloat("Linear", &mBuffer.Kl, 0.0f, 0.5f, "%.3f");
				ImGui::InputFloat("Quadratic", &mBuffer.Kq, 0.0f, 0.5f, "%.3f");
				if(ImGui::Button("Reset"))
				{
					OnReset();
				}
				ImGui::End();
			}
		});

		HEngine::GetInstance().GetUISystemInstance().BindUIComponent(mUIComponent);
	}

	HLight::~HLight()
	{
		HEngine::GetInstance().GetUISystemInstance().UnbindUIComponent(mUIComponent);
	}

	void HLight::Draw(HGraphicSystem& GraphicSystem) const
	{
		mMesh->Draw(GraphicSystem);
	}

	void HLight::Bind(HGraphicSystem& GraphicSystem) const
	{
		mMesh->SetPosition(mBuffer.Position);
		mMesh->SetColor(HColor{ mBuffer.Color });
		mLightBuffer->Update(GraphicSystem, mBuffer);
		mLightBuffer->Bind(GraphicSystem);
	}

	void HLight::OnReset() noexcept
	{
		auto colorFloat3 = [](const DirectX::XMFLOAT4& Value)
		{
			return DirectX::XMFLOAT3{ Value.x, Value.y, Value.z };
		};

		mBuffer.Position = { 5.0f, 4.0f, 0.0f };
		mBuffer.Color = colorFloat3(HColor::White.GetColor());
		mBuffer.Intencity = 1.0f;
		mBuffer.Kc = 1.0f;
		mBuffer.Kl = 0.045f;
		mBuffer.Kq = 0.0075f;
	}

}