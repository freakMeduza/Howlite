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
		uint32_t GetWidth() const noexcept;
		
		/**
		 * Get Height
		 * @return 
		 */
		uint32_t GetHeight() const noexcept;
		
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
		HColor* GetData() noexcept;

		/**
		 * Get Constant Data Pointer
		 * @return 
		 */
		const HColor* GetData() const noexcept;

	private:
		uint32_t mWidth{ 0u };
		uint32_t mHeight{ 0u };

		std::vector<HColor> mPixels;
	};

}
