#pragma once

#include "Bindable.h"

namespace Howlite {

	class HTopology : public HBindable {
	public:
		HTopology(HGraphicSystem& GraphicSystem, D3D11_PRIMITIVE_TOPOLOGY Topology);
		~HTopology() override;

		HTopology(HTopology&&) = delete;
		HTopology(const HTopology&) = delete;
	
		HTopology& operator=(HTopology&&) = delete;
		HTopology& operator=(const HTopology&) = delete;

		void Bind(HGraphicSystem& GraphicSystem)const noexcept override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY mTopology;

	};

}
