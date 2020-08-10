#include "HowlitePCH.h"
#include "VertexBuffer.h"

namespace Howlite {

	HVertexBuffer::HVertexBuffer(HGraphicSystem& GraphicSystem, const HBuffer& Buffer) :
		mStride{ (UINT)Buffer.GetLayout().GetSize() }
	{
		H_DXGI_INFOQUEUE(GraphicSystem)

		D3D11_BUFFER_DESC buffer;

		ZeroMemory(&buffer, sizeof(buffer));

		buffer.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
		buffer.Usage               = D3D11_USAGE_DEFAULT;
		buffer.CPUAccessFlags      = 0u;
		buffer.MiscFlags           = 0u;
		buffer.ByteWidth           = UINT(Buffer.GetSize());
		buffer.StructureByteStride = mStride;

		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&data, sizeof(data));

		data.pSysMem = Buffer.GetData();

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateBuffer(&buffer, &data, &mBuffer))
	}

	HVertexBuffer::~HVertexBuffer()
	{

	}

	void HVertexBuffer::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		const UINT offset = 0u;
		GetContext(GraphicSystem).IASetVertexBuffers(0u, 1u, mBuffer.GetAddressOf(), &mStride, &offset);
	}

}