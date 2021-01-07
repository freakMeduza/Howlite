#include "InputLayout.h"
#include "Device.h"

namespace Howlite {
	
	void InputLayout::Bind(Device* InDevice)
	{
		InDevice->GetDeviceContext()->IASetInputLayout(mInputLayout.Get());
	}

	InputLayout::InputLayout(class Device* InDevice, const std::vector<D3D11_INPUT_ELEMENT_DESC>& InLayoutDescription, ID3DBlob* InVertexShaderBytecode)
	{
		InDevice->GetDevice()->CreateInputLayout(InLayoutDescription.data(), (UINT)InLayoutDescription.size(),
			InVertexShaderBytecode->GetBufferPointer(), InVertexShaderBytecode->GetBufferSize(), &mInputLayout);
	}
}