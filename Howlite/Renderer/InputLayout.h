#pragma once

#include "Bindable.h"
#include "Common/Buffer.h"

namespace Howlite {

	class HInputLayout : public HBindable {
	public:
		HInputLayout(HGraphicSystem& GraphicSystem, const HLayout& Layout, ID3DBlob* VertexShaderbBytecode);
		~HInputLayout() override;

		HInputLayout(HInputLayout&&) = delete;
		HInputLayout(const HInputLayout&) = delete;
	
		HInputLayout& operator=(HInputLayout&&) = delete;
		HInputLayout& operator=(const HInputLayout&) = delete;

		// Bindable interface
		void Bind(HGraphicSystem& GrahpicSystem) const noexcept;

	private:
		HLayout mLayout;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;

	};

}
