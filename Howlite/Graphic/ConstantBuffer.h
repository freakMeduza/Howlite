#pragma once

#include "Bindable.h"

namespace Howlite {

	enum class EConstantBufferSlot : uint8_t 
	{
		Transform,
		Light,
		Material
	};

	struct HTransformData {
	
	};

	struct HLightData {
		alignas(16) DirectX::XMFLOAT3 Position;
		alignas(16) DirectX::XMFLOAT3 Color;
		float Intencity;
		float Kc;
		float Kl;
		float Kq;
	};

	struct HMaterialData {

	};

	template<typename T>
	class HConstantBuffer : public HBindable {
	public:
		HConstantBuffer(HGraphicSystem& GraphicSystem, EConstantBufferSlot Slot) :
			mSlot{ Slot }
		{
			H_DXGI_INFOQUEUE(GraphicSystem)

			D3D11_BUFFER_DESC buffer;

			ZeroMemory(&buffer, sizeof(buffer));

			buffer.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
			buffer.Usage               = D3D11_USAGE_DYNAMIC;
			buffer.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			buffer.MiscFlags           = 0u;
			buffer.ByteWidth           = sizeof(T);
			buffer.StructureByteStride = 0u;

			H_DX_SAFECALL(GetDevice(GraphicSystem).CreateBuffer(&buffer, nullptr, &mBuffer))
		}
		
		HConstantBuffer(HGraphicSystem& GraphicSystem, const T& Data, EConstantBufferSlot Slot) :
			mSlot{ Slot }
		{
			H_DXGI_INFOQUEUE(GraphicSystem)

			D3D11_BUFFER_DESC buffer;

			ZeroMemory(&buffer, sizeof(buffer));

			buffer.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
			buffer.Usage               = D3D11_USAGE_DYNAMIC;
			buffer.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			buffer.MiscFlags           = 0u;
			buffer.ByteWidth           = sizeof(T);
			buffer.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA data;

			ZeroMemory(&data, sizeof(data));

			data.pSysMem = &Data;

			H_DX_SAFECALL(GetDevice(GraphicSystem).CreateBuffer(&buffer, &data, &mBuffer))
		}

		~HConstantBuffer() override = default;

		HConstantBuffer(HConstantBuffer&&) = delete;
		HConstantBuffer(const HConstantBuffer&) = delete;

		HConstantBuffer& operator=(HConstantBuffer&&) = delete;
		HConstantBuffer& operator=(const HConstantBuffer&) = delete;

		/**
		 * @brief Update Constant Buffer
		 * @param GraphicSystem 
		 * @param Data 
		*/
		inline void Update(HGraphicSystem& GraphicSystem, const T& Data)
		{
			D3D11_MAPPED_SUBRESOURCE data;

			GetContext(GraphicSystem).Map(mBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &data);

			std::memcpy(data.pData, &Data, sizeof(T));

			GetContext(GraphicSystem).Unmap(mBuffer.Get(), 0u);
		}

	protected:
		EConstantBufferSlot mSlot;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer;

	};

	template<typename T>
	class HVertexConstantBuffer : public HConstantBuffer<T>
	{
	public:
		HVertexConstantBuffer(HGraphicSystem& GraphicSystem, EConstantBufferSlot Slot) :
			HConstantBuffer<T>(GraphicSystem, Slot)
		{

		}

		HVertexConstantBuffer(HGraphicSystem& GraphicSystem, const T& Data, EConstantBufferSlot Slot) :
			HConstantBuffer<T>(GraphicSystem, Data, Slot)
		{

		}

		HVertexConstantBuffer(HVertexConstantBuffer&&) = delete;
		HVertexConstantBuffer(const HVertexConstantBuffer&) = delete;

		HVertexConstantBuffer& operator=(HVertexConstantBuffer&&) = delete;
		HVertexConstantBuffer& operator=(const HVertexConstantBuffer&) = delete;

		~HVertexConstantBuffer() override = default;

		// Bindable interface
		virtual void Bind(HGraphicSystem& GraphicSystem) const noexcept override
		{
			HBindable::GetContext(GraphicSystem).VSSetConstantBuffers(static_cast<UINT>(HConstantBuffer<T>::mSlot), 1u, HConstantBuffer<T>::mBuffer.GetAddressOf());
		}

	};

	template<typename T>
	class HPixelConstantBuffer : public HConstantBuffer<T>
	{
	public:
		HPixelConstantBuffer(HGraphicSystem& GraphicSystem, EConstantBufferSlot Slot = 0) :
			HConstantBuffer<T>(GraphicSystem, Slot)
		{

		}

		HPixelConstantBuffer(HGraphicSystem& GraphicSystem, const T& Data, EConstantBufferSlot Slot = 0) :
			HConstantBuffer<T>(GraphicSystem, Data, Slot)
		{

		}

		HPixelConstantBuffer(HPixelConstantBuffer&&) = delete;
		HPixelConstantBuffer(const HPixelConstantBuffer&) = delete;

		HPixelConstantBuffer& operator=(HPixelConstantBuffer&&) = delete;
		HPixelConstantBuffer& operator=(const HPixelConstantBuffer&) = delete;

		~HPixelConstantBuffer() override = default;

		// Bindable interface
		virtual void Bind(HGraphicSystem& GraphicSystem) const noexcept override
		{
			HBindable::GetContext(GraphicSystem).PSSetConstantBuffers(static_cast<UINT>(HConstantBuffer<T>::mSlot), 1u, HConstantBuffer<T>::mBuffer.GetAddressOf());
		}

	};
}
