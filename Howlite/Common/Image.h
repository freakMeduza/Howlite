#pragma once

#include "Common/Color.h"

namespace Howlite {

	class HImage {
	public:
		HImage(uint32_t Width, uint32_t Height);
		~HImage();

		HImage(HImage&&) = delete;
		HImage(const HImage&) = delete;

		HImage& operator=(HImage&&) = delete;
		HImage& operator=(const HImage&) = delete;

		/**
		 * Load Image From File
		 * @param File 
		 * @return 
		 */
		static SharedPointer<HImage> LoadFromFile(const std::string& File);
		
		/**
		 * Save
		 * @param File 
		 */
		void Save(const std::string& File) const;
		
		/**
		 * Get Width
		 * @return 
		 */
		uint32_t GetWidth() const;
		
		/**
		 * Get Height
		 * @return 
		 */
		uint32_t GetHeight() const;
		
		/**
		 * Set Pixel
		 * @param X 
		 * @param Y 
		 * @param Color 
		 */
		void SetPixel(size_t X, size_t Y, const HColor& Color);

		/**
		 * Get Pixel
		 * @param X 
		 * @param Y 
		 * @return 
		 */
		HColor GetPixel(size_t X, size_t Y);

		/**
		 * Get Data Pointer
		 * @return 
		 */
		HColor* GetData();

		/**
		 * Get Constant Data Pointer
		 * @return 
		 */
		const HColor* GetData() const;

	private:
		HImage(DirectX::ScratchImage ScratchImage);
		static constexpr DXGI_FORMAT mFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		DirectX::ScratchImage mScratchImage;
	};

}
