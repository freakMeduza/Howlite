#pragma once 

#include "IBindable.h"
#include "Device.h"

namespace Howlite {

	class VertexBuffer : public IBindable {
	public:
		HL_DISABLE_COPY(VertexBuffer);
		// Inherited via IBindable
		virtual void Bind(class Device* InDevice) override;

		template<typename VertexType>
		VertexBuffer(class Device* InDevice, const std::vector<VertexType>& InVertexList) :
			mStride{ sizeof(VertexType) }
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0u;
			bufferDesc.MiscFlags = 0u;
			bufferDesc.ByteWidth = (UINT)(sizeof(VertexType) * InVertexList.size());
			bufferDesc.StructureByteStride = (UINT)sizeof(VertexType);

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = InVertexList.data();

			InDevice->GetDevice()->CreateBuffer(&bufferDesc, &data, &mBuffer);
		}

	private:
		size_t mStride = 0;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mBuffer = nullptr;

	};

}