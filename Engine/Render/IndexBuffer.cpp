#include "IndexBuffer.h"
#include "Device.h"

namespace Howlite {

	void IndexBuffer::Bind(Device* InDevice) 
	{
		InDevice->GetDeviceContext()->IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	IndexBuffer::IndexBuffer(Device* InDevice, const std::vector<uint32_t>& InIndexList) :
		mIndexCount{ InIndexList.size() }
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(mIndexCount * sizeof(uint32_t));
		bufferDesc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = InIndexList.data();

		InDevice->GetDevice()->CreateBuffer(&bufferDesc, &data, &mBuffer);
	}
}