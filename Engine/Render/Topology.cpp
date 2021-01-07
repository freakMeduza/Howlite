#include "Topology.h"
#include "Device.h"

namespace Howlite {

	void Topology::Bind(Device* InDevice)
	{
		InDevice->GetDeviceContext()->IASetPrimitiveTopology(mTopology);
	}

	Topology::Topology(Device* InDevice, D3D11_PRIMITIVE_TOPOLOGY InTopology) :
		mTopology{ InTopology }
	{
		HL_UNUSED(InDevice);
	}

}