#include "HowlitePCH.h"
#include "Texture.h"

namespace Howlite {

	HTexture::HTexture(HGraphicSystem& GraphicSystem, SharedPointer<HImage> Image)
	{
		H_DXGI_INFOQUEUE(GraphicSystem)

		D3D11_TEXTURE2D_DESC description;

		ZeroMemory(&description, sizeof(description));

		description.Width              = Image->GetWidth();
		description.Height             = Image->GetHeight();
		description.MipLevels          = 1;
		description.ArraySize          = 1;
		description.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.SampleDesc.Count   = 1;
		description.SampleDesc.Quality = 0;
		description.Usage              = D3D11_USAGE_DEFAULT;
		description.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		description.CPUAccessFlags     = 0;
		description.MiscFlags          = 0;
		
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&data, sizeof(data));

		data.pSysMem     = Image->GetData();
		data.SysMemPitch = Image->GetWidth() * sizeof(HColor);
		
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateTexture2D(&description, &data, &texture))

		D3D11_SHADER_RESOURCE_VIEW_DESC resource;

		ZeroMemory(&resource, sizeof(resource));

		resource.Format                    = description.Format;
		resource.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource.Texture2D.MostDetailedMip = 0;
		resource.Texture2D.MipLevels       = 1;

		H_DX_SAFECALL(GetDevice(GraphicSystem).CreateShaderResourceView(texture.Get(), &resource, &mResource))
	}

	HTexture::~HTexture()
	{

	}

	void HTexture::Bind(HGraphicSystem& GraphicSystem) const noexcept
	{
		GetContext(GraphicSystem).PSSetShaderResources(0u, 1u, mResource.GetAddressOf());
	}

}