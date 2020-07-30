#include "HowlitePCH.h"
#include "Image.h"

namespace Howlite {

	HImage::HImage(uint32_t Width, uint32_t Height) :
		mWidth{ Width }, mHeight{ Height }
	{
		mPixels.resize(static_cast<size_t>(mWidth * mHeight) * sizeof(HColor));
	}

	HImage::~HImage()
	{

	}

	SharedPointer<HImage> HImage::LoadFromFile(const std::string& File)
	{
		H_ASSERT(false, "Not implemented.");
		return nullptr;
	}

	void HImage::Save(const std::string& File) const
	{
		H_ASSERT(false, "Not implemented.");
	}

	uint32_t HImage::GetWidth() const noexcept
	{
		return mWidth;
	}

	uint32_t HImage::GetHeight() const noexcept
	{
		return mHeight;
	}

	void HImage::SetPixel(size_t X, size_t Y, const HColor& Color)
	{
		H_ASSERT(X < mWidth && Y < mHeight, "Failed to set pixel. Out of range.")
		mPixels[Y * mWidth + X] = Color;
	}

	HColor HImage::GetPixel(size_t X, size_t Y)
	{
		H_ASSERT(X < mWidth&& Y < mHeight, "Failed to set pixel. Out of range.")
		return mPixels[Y * mWidth + X];
	}

	HColor* HImage::GetData() noexcept
	{
		return mPixels.data();
	}

	const HColor* HImage::GetData() const noexcept
	{
		return mPixels.data();
	}

}