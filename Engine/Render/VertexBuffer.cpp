#include "VertexBuffer.h"
#include "Device.h"

namespace Howlite {

	void VertexBuffer::Bind(Device* InDevice)
	{
		const UINT offset = 0u;
		InDevice->GetDeviceContext()->IASetVertexBuffers(0u, 1u, mBuffer.GetAddressOf(), &(UINT)mStride, &offset);
	}

}