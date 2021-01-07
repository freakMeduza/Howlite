#pragma once

#include "IBindable.h"

namespace Howlite {

	class Topology : public IBindable {
	public:
		HL_DISABLE_COPY(Topology);
		// Inherited via IBindable
		virtual void Bind(class Device* InDevice) override;

		Topology(class Device* InDevice, D3D11_PRIMITIVE_TOPOLOGY InTopology);

	private:
		D3D11_PRIMITIVE_TOPOLOGY mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	};

}