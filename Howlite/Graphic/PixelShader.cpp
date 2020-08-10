#include "HowlitePCH.h"
#include "PixelShader.h"

namespace Howlite {

	HPixelShader::HPixelShader(HGraphicSystem& GraphicSystem, const std::wstring& File)
	{
		H_DXGI_INFOQUEUE(GraphicSystem)
		Microsoft::WRL::ComPtr<ID3DBlob> bytecode;
		H_DX_SAFECALL(D3DReadFileToBlob(File.c_str(), &bytecode))
		H_DX_SAFECALL(GetDevice(GraphicSystem).CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, &mShader))
	}

	HPixelShader::~HPixelShader()
	{
	}

	void HPixelShader::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).PSSetShader(mShader.Get(), nullptr, 0u);
	}

}
