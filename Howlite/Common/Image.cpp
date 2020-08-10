#include "HowlitePCH.h"
#include "Image.h"
#include "String.h"

namespace {
	static const std::string EXT_PNG = ".png";
	static const std::string EXT_JPG = ".jpg";
	static const std::string EXT_BMP = ".bmp";
}

namespace Howlite {

	HImage::HImage(uint32_t Width, uint32_t Height) 
	{
		HRESULT hResult = mScratchImage.Initialize2D(mFormat, Width, Height, 1u, 1u);
		if (FAILED(hResult))
		{
			H_ERROR(hResult);
		}
	}

	HImage::~HImage()
	{

	}

	SharedPointer<HImage> HImage::LoadFromFile(const std::string& File)
	{
		using namespace DirectX;

		ScratchImage image;

		HRESULT hResult = LoadFromWICFile(HString::ToWideString(File).c_str(), WIC_FLAGS_IGNORE_SRGB, nullptr, image);
		if(FAILED(hResult))
		{
			H_ERROR(hResult);
		}
		// convert image if needed
		if(image.GetImage(0u, 0u, 0u)->format != mFormat)
		{
			ScratchImage converted;
			hResult = Convert(*image.GetImage(0u, 0u, 0u), mFormat, TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT, converted);
			if(FAILED(hResult))
			{
				H_ERROR(hResult);
			}

			return SharedPointer<HImage>{new HImage{ std::move(converted) }};
		}

		return SharedPointer<HImage>{new HImage{ std::move(image) }};
	}

	void HImage::Save(const std::string& File) const
	{
		using namespace DirectX;

		const auto getCodecID = [](const std::string& File)
		{
			const std::filesystem::path path = File;
			const std::string extension = path.extension().string();
			if(extension == EXT_PNG)
			{
				return WIC_CODEC_PNG;
			}
			else if(extension == EXT_JPG)
			{
				return WIC_CODEC_JPEG;
			}
			else if(extension == EXT_BMP)
			{
				return WIC_CODEC_BMP;
			}

			H_ASSERT(false, "Failed to get codec. Unsupported image format.");
		};

		HRESULT hResult = SaveToWICFile(*mScratchImage.GetImage(0u, 0u, 0u), WIC_FLAGS_NONE, GetWICCodec(getCodecID(File)), HString::ToWideString(File).c_str());
		if(FAILED(hResult))
		{
			H_ERROR(hResult)
		}
	}

	uint32_t HImage::GetWidth() const 
	{
		return (uint32_t)mScratchImage.GetMetadata().width;
	}

	uint32_t HImage::GetHeight() const 
	{
		return (uint32_t)mScratchImage.GetMetadata().height;
	}

	void HImage::SetPixel(size_t X, size_t Y, const HColor& Color)
	{
		H_ASSERT(X < GetWidth(), "Failed to set pixel. 'X' out of range.")
		H_ASSERT(Y < GetHeight(), "Failed to set pixel. 'Y' out of range.")
		auto& data = *mScratchImage.GetImage(0u, 0u, 0u);
		reinterpret_cast<HColor*>(&data.pixels[Y * data.rowPitch])[X] = Color;
	}

	HColor HImage::GetPixel(size_t X, size_t Y)
	{
		H_ASSERT(X < GetWidth(), "Failed to get pixel. 'X' out of range.")
		H_ASSERT(Y < GetHeight(), "Failed to get pixel. 'Y' out of range.")
		auto& data = *mScratchImage.GetImage(0u, 0u, 0u);
		return reinterpret_cast<HColor*>(&data.pixels[Y * data.rowPitch])[X];
	}

	HColor* HImage::GetData() 
	{
		return reinterpret_cast<HColor*>(mScratchImage.GetPixels());
	}

	const HColor* HImage::GetData() const 
	{
		return reinterpret_cast<HColor*>(mScratchImage.GetPixels());
	}

	HImage::HImage(DirectX::ScratchImage ScratchImage) :
		mScratchImage{ std::move(ScratchImage) }
	{
	
	}
}