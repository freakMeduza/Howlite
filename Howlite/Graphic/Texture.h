#pragma once

#include "Bindable.h"
#include "Common/Image.h"

namespace Howlite {

	class HTexture : public HBindable {
	public:
		HTexture(HGraphicSystem& GraphicSystem, SharedPointer<HImage> Image);
		~HTexture() override;

		HTexture(HTexture&&) = delete;
		HTexture(const HTexture&) = delete;
	
		HTexture& operator=(HTexture&&) = delete;
		HTexture& operator=(const HTexture&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mResource;

	};

}
