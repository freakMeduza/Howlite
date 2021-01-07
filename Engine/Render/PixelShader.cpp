#include "PixelShader.h"
#include "Device.h"

namespace Howlite {

	void PixelShader::Bind(Device* InDevice)
	{
		InDevice->GetDeviceContext()->PSSetShader(mPixelShader.Get(), nullptr, 0u);
	}

	PixelShader::PixelShader(Device* InDevice, ID3DBlob* InByteCode)
	{
		InDevice->GetDevice()->CreatePixelShader(InByteCode->GetBufferPointer(), InByteCode->GetBufferSize(), nullptr, &mPixelShader);
	}
}