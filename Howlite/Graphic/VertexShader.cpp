#include "HowlitePCH.h"
#include "VertexShader.h"

namespace Howlite {

	HVertexShader::HVertexShader(HGraphicSystem& GraphicSystem, const std::wstring& File)
	{
		H_DXGI_INFOQUEUE(GraphicSystem)
		H_DX_SAFECALL(D3DReadFileToBlob(File.c_str(), &mBytecode))
		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateVertexShader(mBytecode->GetBufferPointer(), mBytecode->GetBufferSize(), nullptr, &mShader))
	}

	HVertexShader::~HVertexShader()
	{
	}

	void HVertexShader::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).VSSetShader(mShader.Get(), nullptr, 0u);
	}

	ID3DBlob* HVertexShader::GetShaderBytecode() const noexcept
	{
		return mBytecode.Get();
	}
}
