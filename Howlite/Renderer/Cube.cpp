#include "HowlitePCH.h"
#include "Cube.h"

#include "BindableCommon.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"

namespace {
	std::tuple<Howlite::HBuffer, std::vector<uint32_t>> GetCube() noexcept
	{
		Howlite::HBuffer buffer{ Howlite::HLayout{Howlite::HEAttributeType::Position3D}, 24 };

		static constexpr float side = 1.0f;

		buffer[0].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 0 near side
		buffer[1].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });   // 1
		buffer[2].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 2
		buffer[3].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });   // 3
		buffer[4].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side });  // 4 far side
		buffer[5].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });   // 5
		buffer[6].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side });  // 6
		buffer[7].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });   // 7
		buffer[8].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 8 left side
		buffer[9].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 9
		buffer[10].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 10
		buffer[11].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 11
		buffer[12].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });  // 12 right side
		buffer[13].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });  // 13
		buffer[14].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });  // 14
		buffer[15].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });  // 15
		buffer[16].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side }); // 16 bottom side
		buffer[17].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });  // 17
		buffer[18].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 18
		buffer[19].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });  // 19
		buffer[20].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side }); // 20 top side
		buffer[21].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });  // 21
		buffer[22].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 22
		buffer[23].SetAttribute<Howlite::HEAttributeType::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });  // 23

		std::vector<uint32_t> indices{
			 0,  2,  1,  2,  3,  1,
			 4,  5,  7,  4,  7,  6,
			 8, 10,  9, 10, 11,  9,
			12, 13, 15, 12, 15, 14,
			16, 17, 18, 18, 17, 19,
			20, 23, 21, 20, 22, 23
		};

		return { std::move(buffer), std::move(indices) };
	}
}

namespace Howlite {

	HCube::HCube(const std::string& Name, HGraphicSystem& GraphicSystem) :
		mName{ Name }, mPosition{ 0.0f, 0.0f, 0.0f }, mRotation{ 0.0f, 0.0f, 0.0f }
	{
		auto [buffer, indices] = GetCube();

		AddBind(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		AddBind(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));
		AddBind(CreateSharedPointer<HVertexShader>(GraphicSystem, L"VertexShader.cso"));
		AddBind(CreateSharedPointer<HPixelShader>(GraphicSystem, L"PixelShader.cso"));
		AddBind(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(CreateSharedPointer<HTransformBuffer>(GraphicSystem, *this));
		AddBind(CreateSharedPointer<HPixelConstantBuffer<ColorBuffer>>(GraphicSystem, mColorBuffer, 0u));
		if(auto vertexShader = QueryBindable<HVertexShader>())
		{
			AddBind(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		}
	}

	DirectX::XMMATRIX HCube::GetTransform() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z)
			* DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}

	void HCube::Update(float DeltaTime) noexcept
	{
		H_UNUSED(DeltaTime)
		HGraphicSystem& graphicSystem = HEngine::GetInstance().GetGraphicSystemInstance();
		if (auto ptr = QueryBindable<HPixelConstantBuffer<ColorBuffer>>())
		{
			ptr->Update(graphicSystem, mColorBuffer);
		}
	}

	void HCube::SpwanUIWindow()
	{
		if(ImGui::Begin(mName.c_str(), (bool*)nullptr))
		{
			ImGui::Text("Position:");
			ImGui::SliderFloat("x", &mPosition.x, -50.0f, 50.0f, "%.1f");
			ImGui::SliderFloat("y", &mPosition.y, -50.0f, 50.0f, "%.1f");
			ImGui::SliderFloat("z", &mPosition.z, -50.0f, 50.0f, "%.1f");
			if(ImGui::Button("Reset position", ImVec2{150, 25}))
			{
				OnResetPosition();
			}
			ImGui::Text("Rotation:");
			ImGui::SliderAngle("roll", &mRotation.x, -180.0f, 180.0f);
			ImGui::SliderAngle("pitch", &mRotation.y, -180.0f, 180.0f);
			ImGui::SliderAngle("yaw", &mRotation.z, -180.0f, 180.0f);
			if (ImGui::Button("Reset rotation", ImVec2{ 150, 25 }))
			{
				OnResetRotation();
			}
			ImGui::ColorEdit3("Color", (float*)&mColorBuffer);
			if (ImGui::Button("Reset color", ImVec2{ 150, 25 }))
			{
				OnResetColor();
			}
			ImGui::End();
		}
	}

	void HCube::OnResetPosition() noexcept
	{
		mPosition.x = 0.0f;
		mPosition.y = 0.0f;
		mPosition.z = 0.0f;
	}

	void HCube::OnResetRotation() noexcept
	{
		mRotation.x = 0.0f;
		mRotation.y = 0.0f;
		mRotation.z = 0.0f;
	}

	void HCube::OnResetColor() noexcept
	{
		mColorBuffer = ColorBuffer{};
	}

}