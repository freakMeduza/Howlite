#include "HowlitePCH.h"
#include "IndexBuffer.h"

namespace Howlite {

	HIndexBuffer::HIndexBuffer(HGraphicSystem& GraphicSystem, const std::vector<uint32_t>& Indices) :
		mCount{ (UINT)Indices.size() }
	{
		H_DXGI_INFOQUEUE(GraphicSystem)

		D3D11_BUFFER_DESC buffer;

		ZeroMemory(&buffer, sizeof(buffer));

		buffer.BindFlags           = D3D11_BIND_INDEX_BUFFER;
		buffer.Usage               = D3D11_USAGE_DEFAULT;
		buffer.CPUAccessFlags      = 0u;
		buffer.MiscFlags           = 0u;
		buffer.ByteWidth           = UINT(mCount * sizeof(uint32_t));
		buffer.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&data, sizeof(data));

		data.pSysMem = Indices.data();

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateBuffer(&buffer, &data, &mBuffer))
	}

	HIndexBuffer::~HIndexBuffer()
	{

	}

	void HIndexBuffer::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).IASetIndexBuffer(mBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
	}

	UINT HIndexBuffer::GetIndicesCount() const noexcept
	{
		return mCount;
	}

}