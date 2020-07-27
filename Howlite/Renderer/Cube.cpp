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

	HCube::HCube(HGraphicSystem& GraphicSystem) :
		mPosition{ 0.0f, 0.0f, 0.0f }, mRotation{ 0.0f, 0.0f, 0.0f }
	{
		auto [buffer, indices] = GetCube();

		AddBind(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		AddBind(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));
		auto vertexShader = CreateSharedPointer<HVertexShader>(GraphicSystem, L"VertexShader.cso");
		AddBind(CreateSharedPointer<HPixelShader>(GraphicSystem, L"PixelShader.cso"));
		AddBind(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		AddBind(vertexShader);
		const HWindow& window = HEngine::GetInstance().GetWindowInstance();
		mTransform.ModelMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
		mTransform.ViewMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
														  DirectX::XMVectorZero(),
														  DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		mTransform.ProjectionMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, (float)window.GetHeight() / (float)window.GetWidth(), 0.5f, 100.0f));
		AddBind(CreateSharedPointer<HVertexConstantBuffer<Transform>>(GraphicSystem, mTransform, 0u));
		mColor.R = 1.0f;
		mColor.G = 0.0f;
		mColor.B = 0.0f;
		mColor.A = 1.0f;
		AddBind(CreateSharedPointer<HPixelConstantBuffer<Color>>(GraphicSystem, mColor, 0u));
	}

	DirectX::XMMATRIX HCube::GetTransform() const noexcept
	{
		return mTransform.ModelMatrix;
	}

	void HCube::Update(float DeltaTime) noexcept
	{
		H_UNUSED(DeltaTime)
		const HWindow& window = HEngine::GetInstance().GetWindowInstance();
		HGraphicSystem& graphicSystem = HEngine::GetInstance().GetGraphicSystemInstance();
		const float width = (float)window.GetWidth();
		const float height = (float)window.GetHeight();
		const auto& transform = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z) 
			* DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
		mTransform.ModelMatrix = DirectX::XMMatrixTranspose(transform);
		mTransform.ProjectionMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, 0.5f, 100.0f));
		if(auto ptr = QueryBindable<HVertexConstantBuffer<Transform>>())
		{
			ptr->Update(graphicSystem, mTransform);
		}
		if (auto ptr = QueryBindable<HPixelConstantBuffer<Color>>())
		{
			ptr->Update(graphicSystem, mColor);
		}
	}

	void HCube::SpwanUIWindow()
	{
		if(ImGui::Begin("Cube Entity", (bool*)nullptr))
		{
			ImGui::Text("Position:");
			ImGui::SliderFloat("x", &mPosition.x, -50.0f, 50.0f, "%.1f");
			ImGui::SliderFloat("y", &mPosition.y, -50.0f, 50.0f, "%.1f");
			ImGui::SliderFloat("z", &mPosition.z, -50.0f, 50.0f, "%.1f");
			ImGui::Text("Rotation:");
			ImGui::SliderAngle("roll", &mRotation.x, -180.0f, 180.0f);
			ImGui::SliderAngle("pitch", &mRotation.y, -180.0f, 180.0f);
			ImGui::SliderAngle("yaw", &mRotation.z, -180.0f, 180.0f);
			if(ImGui::Button("Reset", ImVec2{100, 25}))
			{
				OnReset();
			}
			ImGui::ColorEdit3("Color", (float*)&mColor);
			ImGui::End();
		}
	}

	void HCube::OnReset() noexcept
	{
		mPosition.x = 0.0f;
		mPosition.y = 0.0f;
		mPosition.z = 0.0f;
		mRotation.x = 0.0f;
		mRotation.y = 0.0f;
		mRotation.z = 0.0f;
	}

}