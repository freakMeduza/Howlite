#include "HowlitePCH.h"
#include "Cube.h"
#include "BindableCommon.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "UI/UISystem.h"

namespace {
	void Transform(DirectX::FXMMATRIX Matrix, Howlite::HBuffer& Buffer)
	{
		for (int i = 0; i < Buffer.GetElementCount(); i++)
		{
			auto& position = Buffer[i].GetAttribute<Howlite::EAttributeType::Position3D>();
			DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), Matrix));
		}
	}

	void SetNormalsIndependentFlat(Howlite::HBuffer& Buffer, const std::vector<uint32_t>& Indices)
	{
		using namespace DirectX;
		
		for (size_t i = 0; i < Indices.size(); i += 3)
		{
			auto v0 = Buffer[Indices[i]];
			auto v1 = Buffer[Indices[i + 1]];
			auto v2 = Buffer[Indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.GetAttribute<Howlite::EAttributeType::Position3D>());
			const auto p1 = XMLoadFloat3(&v1.GetAttribute<Howlite::EAttributeType::Position3D>());
			const auto p2 = XMLoadFloat3(&v2.GetAttribute<Howlite::EAttributeType::Position3D>());

			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.GetAttribute<Howlite::EAttributeType::Normal3D>(), n);
			XMStoreFloat3(&v1.GetAttribute<Howlite::EAttributeType::Normal3D>(), n);
			XMStoreFloat3(&v2.GetAttribute<Howlite::EAttributeType::Normal3D>(), n);
		}
	}


	[[maybe_unused]] static std::tuple<Howlite::HBuffer, std::vector<uint32_t>> GetCube() noexcept
	{
		using Type = Howlite::EAttributeType;

		Howlite::HBuffer buffer{ Howlite::HLayout{Type::Position3D, Type::Normal3D}, 24 };

		static constexpr float side = 1.0f;

		buffer[0].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 0 near side
		buffer[1].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });   // 1
		buffer[2].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 2
		buffer[3].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });   // 3
		buffer[4].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side });  // 4 far side
		buffer[5].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });   // 5
		buffer[6].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side });  // 6
		buffer[7].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });   // 7
		buffer[8].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side });  // 8 left side
		buffer[9].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side });  // 9
		buffer[10].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 10
		buffer[11].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 11
		buffer[12].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });  // 12 right side
		buffer[13].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });  // 13
		buffer[14].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });  // 14
		buffer[15].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });  // 15
		buffer[16].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side, -side }); // 16 bottom side
		buffer[17].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side, -side });  // 17
		buffer[18].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side, -side,  side }); // 18
		buffer[19].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side, -side,  side });  // 19
		buffer[20].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side, -side }); // 20 top side
		buffer[21].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side, -side });  // 21
		buffer[22].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ -side,  side,  side }); // 22
		buffer[23].SetAttribute<Type::Position3D>(DirectX::XMFLOAT3{ side,  side,  side });  // 23

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

	[[maybe_unused]] static std::tuple<Howlite::HBuffer, std::vector<uint32_t>> GetSkinnedCube() noexcept
	{
		Howlite::HBuffer buffer{ Howlite::HLayout{Howlite::EAttributeType::Position3D, Howlite::EAttributeType::UV2D}, 14 };

		static constexpr float side = 1.0f;

		buffer[0].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,-side,-side });
		buffer[0].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 2.0f / 3.0f,0.0f / 4.0f });
		buffer[1].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,-side,-side });
		buffer[1].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 1.0f / 3.0f,0.0f / 4.0f });
		buffer[2].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,side,-side });
		buffer[2].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 2.0f / 3.0f,1.0f / 4.0f });
		buffer[3].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,side,-side });
		buffer[3].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 1.0f / 3.0f,1.0f / 4.0f });
		buffer[4].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,-side,side });
		buffer[4].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 2.0f / 3.0f,3.0f / 4.0f });
		buffer[5].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,-side,side });
		buffer[5].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 1.0f / 3.0f,3.0f / 4.0f });
		buffer[6].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,side,side });
		buffer[6].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 2.0f / 3.0f,2.0f / 4.0f });
		buffer[7].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,side,side });
		buffer[7].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 1.0f / 3.0f,2.0f / 4.0f });
		buffer[8].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,-side,-side });
		buffer[8].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 2.0f / 3.0f,4.0f / 4.0f });
		buffer[9].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,-side,-side });
		buffer[9].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 1.0f / 3.0f,4.0f / 4.0f });
		buffer[10].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,-side,-side });
		buffer[10].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 3.0f / 3.0f,1.0f / 4.0f });
		buffer[11].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ -side,-side,side });
		buffer[11].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 3.0f / 3.0f,2.0f / 4.0f });
		buffer[12].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,-side,-side });
		buffer[12].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 0.0f / 3.0f,1.0f / 4.0f });
		buffer[13].SetAttribute<Howlite::EAttributeType::Position3D>(DirectX::XMFLOAT3{ side,-side,side });
		buffer[13].SetAttribute<Howlite::EAttributeType::UV2D>(DirectX::XMFLOAT2{ 0.0f / 3.0f,2.0f / 4.0f });

		std::vector<uint32_t> indices{
			 0,  2,  1,  2,  3,  1,
			 4,  8,  5,  5,  8,  9,
			 2,  6,  3,  3,  6,  7,
			 4,  5,  7,  4,  7,  6,
			 2, 10, 11,  2, 11,  6,
			12,  3,  7, 12,  7, 13
		};

		return { std::move(buffer), std::move(indices) };
	}
}

namespace Howlite {

	HCube::HCube(HGraphicSystem& GraphicSystem) :
		mPosition{ 0.0f, 0.0f, 0.0f }, mRotation{ 0.0f, 0.0f, 0.0f }
	{
		auto [buffer, indices] = GetCube();
		SetNormalsIndependentFlat(buffer, indices);
		AddBind(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		AddBind(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));
		AddBind(CreateSharedPointer<HVertexShader>(GraphicSystem, L"PhongVertexShader.cso"));
		AddBind(CreateSharedPointer<HPixelShader>(GraphicSystem, L"PhongPixelShader.cso"));
		AddBind(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(CreateSharedPointer<HTransformBuffer>(GraphicSystem, *this, EConstantBufferSlot::Transform));
		if(auto vertexShader = QueryBindable<HVertexShader>())
		{
			AddBind(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		}
		
		ColorBuffer color
		{
			{
				HColor::ClearWater.GetRed<float>(),
				HColor::ClearWater.GetGreen<float>(),
				HColor::ClearWater.GetBlue<float>()
			},
			30.0f, 
			0.6f
		}; 

		AddBind(CreateSharedPointer<HPixelConstantBuffer<ColorBuffer>>(GraphicSystem, color, EConstantBufferSlot::Material));
	}

	DirectX::XMMATRIX HCube::GetTransform() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(mRotation.y, mRotation.z, mRotation.x)
			* DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
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

}