#pragma once

#include "IBindable.h"
#include "Device.h"

namespace Howlite {

	enum class ShaderStage : uint8_t {
		Vertex,
		Pixel
	};

	template<typename ValueType, ShaderStage BufferShaderStage, size_t BufferIndex = 0>
	class TConstantBuffer : public IBindable {
		static constexpr size_t SIZE = sizeof(ValueType);
		static constexpr size_t INDEX = BufferIndex;
		static constexpr ShaderStage SHADER_STAGE = BufferShaderStage;

	public:
		HL_DISABLE_COPY(TConstantBuffer);

		TConstantBuffer(Device* InDevice)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0u;
			bufferDesc.ByteWidth = SIZE;
			bufferDesc.StructureByteStride = 0u;

			InDevice->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &mBuffer);
		}

		TConstantBuffer(Device* InDevice, const ValueType& InValue)
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0u;
			bufferDesc.ByteWidth = SIZE;
			bufferDesc.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = &InValue;

			InDevice->GetDevice()->CreateBuffer(&bufferDesc, &data, &mBuffer);
		}

		// Inherited via IBindable
		virtual void Bind(Device* InDevice) override
		{
			switch (SHADER_STAGE)
			{
				case ShaderStage::Vertex:
					InDevice->GetDeviceContext()->VSSetConstantBuffers((UINT)INDEX, 1u, mBuffer.GetAddressOf());
					break;
				case ShaderStage::Pixel:
					InDevice->GetDeviceContext()->PSSetConstantBuffers((UINT)INDEX, 1u, mBuffer.GetAddressOf());
					break;
			}
		}

		void Update(Device* InDevice, const ValueType& InValue)
		{
			D3D11_MAPPED_SUBRESOURCE data;
			InDevice->GetDeviceContext()->Map(mBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &data);
			std::memcpy(data.pData, &InValue, SIZE);
			InDevice->GetDeviceContext()->Unmap(mBuffer.Get(), 0u);
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer = nullptr;

	};

}