#pragma once

#include "IBindable.h"

namespace Howlite {

	class InputLayout : public IBindable {
	public:
		HL_DISABLE_COPY(InputLayout);
		// Inherited via IBindable
		virtual void Bind(class Device* InDevice) override;

		InputLayout(class Device* InDevice, const std::vector<D3D11_INPUT_ELEMENT_DESC>& InLayoutDescription, ID3DBlob* InVertexShaderBytecode);

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout = nullptr;

	};

}