#pragma once

#include "Bindable.h"

namespace Howlite {

	class HVertexShader : public HBindable {
	public:
		HVertexShader(HGraphicSystem& GraphicSystem, const std::wstring& File);
		~HVertexShader() override;

		HVertexShader(HVertexShader&&) = delete;
		HVertexShader(const HVertexShader&) = delete;
	
		HVertexShader& operator=(HVertexShader&&) = delete;
		HVertexShader& operator=(const HVertexShader&) = delete;
		
		// Bindable interface
		void Bind(HGraphicSystem& GraphicSystem) const noexcept override;

		/**
		 * Get Shader Bytecode
		 * @return 
		 */
		ID3DBlob* GetShaderBytecode() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> mBytecode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mShader;

	};

}