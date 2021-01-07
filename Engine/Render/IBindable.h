#pragma once

#include "Engine/Platform.h"

namespace Howlite {

	class IBindable {
	public:
		HL_INTERFACE_DISABLE_COPY(IBindable);

		virtual void Bind(class Device* InDevice) = 0;
	};

}