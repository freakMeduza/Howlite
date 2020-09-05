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
		mMesh = CreateScopedPointer<HSphere>(GraphicSystem, mLightData.Position, Radius);
		mLightBuffer = CreateScopedPointer<HPixelConstantBuffer<HLightData>>(GraphicSystem, mLightData, EConstantBufferSlot::Light);
	}

	HLight::~HLight()
	{

	}

	void HLight::Draw(HGraphicSystem& GraphicSystem) const
	{
		mMesh->Draw(GraphicSystem);
	}

	void HLight::Bind(HGraphicSystem& GraphicSystem) const
	{
		mMesh->SetPosition(mLightData.Position);
		mMesh->SetColor(HColor{ mLightData.Color });
		mLightBuffer->Bind(GraphicSystem);
		mLightBuffer->Update(GraphicSystem, mLightData);
	}

	void HLight::DrawUIWindow(bool* IsOpen) noexcept
	{
		const ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

		if(!ImGui::Begin("Light", IsOpen, flags))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Position");
		ImGui::SliderFloat("x", &mLightData.Position.x, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("y", &mLightData.Position.y, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("z", &mLightData.Position.z, -1000.0f, 1000.0f, "%.1f");
		ImGui::Separator();
		ImGui::Text("Appearance");
		ImGui::SliderFloat("Intencity", &mLightData.Intencity, 0.1f, 200.0f, "%.2f", 2.0f);
		ImGui::ColorEdit3("Color", &mLightData.Color.x);
		ImGui::Separator();
		ImGui::Text("Modifiers");
		ImGui::SliderFloat("Constant", &mLightData.Kc, 0.05f, 10.0f, "%.2f", 4.0f);
		ImGui::SliderFloat("Linear", &mLightData.Kl, 0.0001f, 4.0f, "%.4f", 8.0f);
		ImGui::SliderFloat("Quadratic", &mLightData.Kq, 0.0000001f, 10.0f, "%.7f", 10.0f);
		ImGui::Separator();
		if (ImGui::Button("Reset"))
		{
			OnReset();
		}
		ImGui::Separator();
		ImGui::End();
	}

	void HLight::OnReset() noexcept
	{
		mLightData.Position  = { 140.0f, 140.0f, -350.0f };
		mLightData.Color     = HColor::White.GetColorFloat3();
		mLightData.Intencity = 200.0f;
		mLightData.Kc        = 1.0f;
		mLightData.Kl        = 0.045f;
		mLightData.Kq        = 0.0075f;
	}

}