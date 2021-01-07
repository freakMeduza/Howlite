#include "VertexShader.h"
#include "Device.h"

namespace Howlite {

	void VertexShader::Bind(Device* InDevice)
	{
		InDevice->GetDeviceContext()->VSSetShader(mVertexShader.Get(), nullptr, 0u);
	}

	VertexShader::VertexShader(Device* InDevice, ID3DBlob* InByteCode) :
		mByteCode{ InByteCode }
	{
		InDevice->GetDevice()->CreateVertexShader(mByteCode->GetBufferPointer(), mByteCode->GetBufferSize(), nullptr, &mVertexShader);
	}

}