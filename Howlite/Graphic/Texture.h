#pragma once

#include "Bindable.h"
#include "Common/Image.h"

namespace Howlite {

	enum class ETextureSlot : uint8_t
	{
		Diffuse,
		Normal,
		Specular
	};

	class HTexture : public HBindable {
	public:
		HTexture(HGraphicSystem& GraphicSystem, SharedPointer<HImage> Image, ETextureSlot Slot);
		~HTexture() override;

		HTexture(HTexture&&) = delete;
		HTexture(const HTexture&) = delete;
	
		HTexture& operator=(HTexture&&) = delete;
		HTexture& operator=(const HTexture&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept override;

	private:
		ETextureSlot mSlot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mResource;

	};

}
