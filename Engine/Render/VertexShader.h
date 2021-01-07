#pragma once

#include "IBindable.h"

namespace Howlite {

	class VertexShader : public IBindable {
	public:
		HL_DISABLE_COPY(VertexShader);
		// Inherited via IBindable
		virtual void Bind(Device* InDevice) override;

		VertexShader(class Device* InDevice, ID3DBlob* InByteCode);

		inline ID3DBlob* GetByteCode() const noexcept { return mByteCode.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> mByteCode = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader = nullptr;

	};


}