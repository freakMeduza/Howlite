#pragma once

#include "Bindable.h"

namespace Howlite {

	class HPixelShader : public HBindable
	{
	public:
		HPixelShader(HGraphicSystem& GraphicSystem, const std::wstring& File);
		~HPixelShader() override;

		HPixelShader(HPixelShader&&) = delete;
		HPixelShader(const HPixelShader&) = delete;

		HPixelShader& operator=(HPixelShader&&) = delete;
		HPixelShader& operator=(const HPixelShader&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept override;

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mShader;

	};

}