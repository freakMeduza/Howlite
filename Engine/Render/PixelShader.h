#pragma once

#include "IBindable.h"

namespace Howlite {

	class PixelShader : public IBindable {
	public:
		HL_DISABLE_COPY(PixelShader);
		// Inherited via IBindable
		virtual void Bind(Device* InDevice) override;

		PixelShader(class Device* InDevice, ID3DBlob* InByteCode);

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader = nullptr;

	};


}