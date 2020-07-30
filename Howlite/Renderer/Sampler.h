#pragma once

#include "Bindable.h"

namespace Howlite {

	class HSampler : HBindable {
	public:
		HSampler(HGraphicSystem& GraphicSystem);
		~HSampler() override;

		HSampler(HSampler&&) = delete;
		HSampler(const HSampler&) = delete;

		HSampler& operator=(HSampler&&) = delete;
		HSampler& operator=(const HSampler&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSampler;

	};

}
