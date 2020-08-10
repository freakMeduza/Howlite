#include "HowlitePCH.h"
#include "Topology.h"

namespace Howlite {

	HTopology::HTopology(HGraphicSystem& GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY Topology) :
		mTopology{ Topology }
	{
		H_UNUSED(GraphicSystem)
	}

	HTopology::~HTopology()
	{

	}

	void HTopology::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).IASetPrimitiveTopology(mTopology);
	}

}