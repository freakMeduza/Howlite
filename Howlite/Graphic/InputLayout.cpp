#include "HowlitePCH.h"
#include "InputLayout.h"

namespace Howlite {

	HInputLayout::HInputLayout(HGraphicSystem& GraphicSystem, const HLayout& Layout, ID3DBlob* VertexShaderbBytecode) :
		mLayout{ Layout }
	{
		H_DXGI_INFOQUEUE(GraphicSystem)

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateInputLayout(
			mLayout.GetInputLayoutDescription().data(),
			(UINT)mLayout.GetAttributeCount(),
			VertexShaderbBytecode->GetBufferPointer(),
			VertexShaderbBytecode->GetBufferSize(),
			&mInputLayout))
	}

	HInputLayout::~HInputLayout()
	{

	}

	void HInputLayout::Bind(HGraphicSystem& GrahpicSystem) const noexcept
	{
		GetContext(GrahpicSystem).IASetInputLayout(mInputLayout.Get());
	}

}