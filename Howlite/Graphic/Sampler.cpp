#include "HowlitePCH.h"
#include "Sampler.h"

namespace Howlite {

	HSampler::HSampler(HGraphicSystem& GraphicSystem)
	{
		H_DXGI_INFOQUEUE(GraphicSystem)

		D3D11_SAMPLER_DESC sampler;

		ZeroMemory(&sampler, sizeof(sampler));

		sampler.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateSamplerState(&sampler, &mSampler));
	}

	HSampler::~HSampler()
	{

	}

	void HSampler::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).PSSetSamplers(0u, 1u, mSampler.GetAddressOf());
	}

}