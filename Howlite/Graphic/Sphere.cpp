#include "HowlitePCH.h"
#include "Sphere.h"
#include "BindableCommon.h"
#include "Common/Math.h"
#include "Common/String.h"
#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "ShaderTable.h"

namespace {
	[[maybe_unused]] static void Transform(DirectX::FXMMATRIX Matrix, Howlite::HBuffer& Buffer)
	{
		for (int i = 0; i < Buffer.GetElementCount(); i++)
		{
			auto& position = Buffer[i].GetAttribute<Howlite::EAttributeType::Position3D>();
			DirectX::XMStoreFloat3(&position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&position), Matrix));
		}
	}

	[[maybe_unused]] static std::tuple<Howlite::HBuffer, std::vector<uint32_t>> GetSphere() noexcept
	{
		Howlite::HBuffer buffer{ Howlite::HLayout{Howlite::EAttributeType::Position3D}};

		using namespace DirectX;

		const uint32_t latDiv = 32;
		const uint32_t longDiv = 32;

		constexpr float radius = 1.0f;
		const auto base = XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		
		const float lattitudeAngle = H_PI / latDiv;
		const float longitudeAngle = H_2PI / longDiv;

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = XMVector3Transform(base, XMMatrixRotationX(lattitudeAngle * iLat));
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				XMFLOAT3 calculatedPos;
				auto v = XMVector3Transform(latBase, XMMatrixRotationZ(longitudeAngle * iLong));
				XMStoreFloat3(&calculatedPos, v);
				buffer.EmplaceBack(calculatedPos);
			}
		}

		const auto iNorthPole = (uint32_t)buffer.GetSize();
		{
			XMFLOAT3 northPos;
			XMStoreFloat3(&northPos, base);
			buffer.EmplaceBack(northPos);
		}
		const auto iSouthPole = (uint32_t)buffer.GetSize();
		{
			XMFLOAT3 southPos;
			XMStoreFloat3(&southPos, XMVectorNegate(base));
			buffer.EmplaceBack(southPos);
		}

		const auto calcIdx = [latDiv, longDiv](uint32_t iLat, uint32_t iLong)
		{
			return iLat * longDiv + iLong;
		};

		std::vector<uint32_t> indices;
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indices.push_back(calcIdx(iLat, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat, iLong + 1));
				indices.push_back(calcIdx(iLat + 1, iLong));
				indices.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			indices.push_back(calcIdx(iLat, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat, 0));
			indices.push_back(calcIdx(iLat + 1, longDiv - 1));
			indices.push_back(calcIdx(iLat + 1, 0));
		}
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, iLong));
			indices.push_back(calcIdx(0, iLong + 1));
		
			indices.push_back(calcIdx(latDiv - 2, iLong + 1));
			indices.push_back(calcIdx(latDiv - 2, iLong));
			indices.push_back(iSouthPole);
		}
		indices.push_back(iNorthPole);
		indices.push_back(calcIdx(0, longDiv - 1));
		indices.push_back(calcIdx(0, 0));
		
		indices.push_back(calcIdx(latDiv - 2, 0));
		indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indices.push_back(iSouthPole);

		return { std::move(buffer), std::move(indices) };
	}
}

namespace Howlite {

	HSphere::HSphere(HGraphicSystem& GraphicSystem, const DirectX::XMFLOAT3& Position, float Radius) :
		mPosition{ Position }, mColor{ HColor::White.GetColorFloat4() }
	{
		auto [buffer, indices] = GetSphere();

		Transform(DirectX::XMMatrixScaling(Radius, Radius, Radius), buffer);

		AddBind(CreateSharedPointer<HVertexBuffer>(GraphicSystem, buffer));
		AddBind(CreateSharedPointer<HIndexBuffer>(GraphicSystem, indices));
		AddBind(CreateSharedPointer<HVertexShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Solid_VS]));
		AddBind(CreateSharedPointer<HPixelShader>(GraphicSystem, HShaderTable::GetInstance()[EShaderType::Solid_PS]));
		AddBind(CreateSharedPointer<HTransformBuffer>(GraphicSystem, *this, EConstantBufferSlot::Transform));
		AddBind(CreateSharedPointer<HTopology>(GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		//AddBind(CreateSharedPointer<HPixelConstantBuffer<DirectX::XMFLOAT4>>(GraphicSystem, mColor, EConstantBufferSlot::Material));
		HMaterialData data;
		data.Color = { mColor.x, mColor.y, mColor.z };
		AddBind(CreateSharedPointer<HPixelConstantBuffer<HMaterialData>>(GraphicSystem, data, EConstantBufferSlot::Material));

		if (auto vertexShader = QueryBindable<HVertexShader>())
		{
			AddBind(CreateSharedPointer<HInputLayout>(GraphicSystem, buffer.GetLayout(), vertexShader->GetShaderBytecode()));
		}
	}

	DirectX::XMMATRIX HSphere::GetTransform() const noexcept
	{
		return DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	}

	void HSphere::SetColor(const HColor& Color) noexcept
	{
		mColor = Color.GetColorFloat4();
		HMaterialData data;
		data.Color = Color.GetColorFloat3();

		if (auto constantBuffer = QueryBindable<HPixelConstantBuffer<HMaterialData>>())
		{
			constantBuffer->Update(HEngine::GetInstance().GetGraphicSystemInstance(), data);
		}
	}

	HColor HSphere::GetColor() const noexcept
	{
		return HColor{ mColor.x, mColor.y, mColor.z, mColor.w };
	}

	void HSphere::SetPosition(const DirectX::XMFLOAT3& Position) noexcept
	{
		mPosition = Position;
	}

	DirectX::XMFLOAT3 HSphere::GetPosition() const noexcept
	{
		return mPosition;
	}

}