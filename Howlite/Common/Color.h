#pragma once

namespace Howlite {

	class HColor {
	public:
		// Basic
		static const HColor Transparent;
		static const HColor Black;
		static const HColor White;
		static const HColor DarkGray;
		static const HColor Red;
		static const HColor Green;
		static const HColor Blue;
		static const HColor Yellow;
		static const HColor Magenta;
		static const HColor LightBlue;
		// Red		 
		static const HColor DarkRed;
		static const HColor Salmon;
		static const HColor LightSalmon;
		static const HColor Crimson;
		static const HColor FireBrick;
		// Pink		 
		static const HColor Pink;
		static const HColor HotPink;
		static const HColor DeepPink;
		static const HColor MediumVioletRed;
		static const HColor PaleVioletRed;
		// Orange	 
		static const HColor Orange;
		static const HColor DarkOrange;
		static const HColor OrangeRed;
		static const HColor Tomato;
		static const HColor Coral;
		// Yellow	 
		static const HColor Gold;
		static const HColor LightYellow;
		static const HColor Peach;
		static const HColor Moccasin;
		// Purple	 
		static const HColor Lavender;
		static const HColor Violet;
		static const HColor DarkViolet;
		static const HColor DarkMagenta;
		static const HColor Purple;
		static const HColor Indigo;
		static const HColor SlateBlue;
		static const HColor DarkSlateBlue;
		static const HColor MediumSlateBlue;
		// Green	 
		static const HColor GreenYellow;
		static const HColor Chartreuse;
		static const HColor LimeGreen;
		static const HColor PaleGreen;
		static const HColor MediumSpringGreen;
		static const HColor SpringGreen;
		static const HColor SeaGreen;
		static const HColor ForestGreen;
		static const HColor DarkGreen;
		static const HColor YellowGreen;
		static const HColor OliveDrab;
		static const HColor Olive;
		static const HColor MediumAquamarine;
		static const HColor LightSeaGreen;
		static const HColor DarkCyan;
		static const HColor Teal;
		// Blue		 
		static const HColor LightCyan;
		static const HColor Aquamarine;
		static const HColor Turquoise;
		static const HColor CadetBlue;
		static const HColor SteelBlue;
		static const HColor LightSteelBlue;
		static const HColor ClearWater;
		static const HColor SkyBlue;
		static const HColor LightSkyBlue;
		static const HColor DeepSkyBlue;
		static const HColor DodgerBlue;
		static const HColor RoyalBlue;
		static const HColor MediumBlue;
		static const HColor DarkBlue;
		static const HColor Navy;
		static const HColor MidnightBlue;
		static const HColor PhyreBlue;
		// Brown	 
		static const HColor CornSilk;
		static const HColor Bisque;
		static const HColor Wheat;
		static const HColor BurlyWood;
		static const HColor Tan;
		static const HColor RosyBrown;
		static const HColor SandyBrown;
		static const HColor GoldenRod;
		static const HColor Peru;
		static const HColor SaddleBrown;
		static const HColor Sienna;
		static const HColor Brown;
		static const HColor Maroon;

		HColor() = default;
		~HColor() = default;

		HColor(uint32_t Color);
		HColor(const DirectX::XMFLOAT3& Color);
		HColor(const DirectX::XMFLOAT4& Color);
		HColor(float RedChannel, float GreenChannel, float BlueChannel);
		HColor(float RedChannel, float GreenChannel, float BlueChannel, float AlphaChannel);
		HColor(uint8_t RedChannel, uint8_t GreenChannel, uint8_t BlueChannel);
		HColor(uint8_t RedChannel, uint8_t GreenChannel, uint8_t BlueChannel, uint8_t AlphaChannel);

		HColor(HColor&&) = default;
		HColor(const HColor&) = default;

		HColor& operator=(HColor&&) = default;
		HColor& operator=(const HColor&) = default;

		inline operator uint32_t() const noexcept
		{
			return mColor;
		}

		/**
		 * Get Red Channel Value
		 * @return 
		 */
		template<typename T>
		inline T GetRed() const
		{
			if constexpr (std::is_same_v<T, uint8_t>)
			{
				return mColor & 0xFFu;
			}
			else if constexpr (std::is_same_v<T, float> )
			{
				return (mColor & 0xFFu) / 255.0f;
			}
			else
			{
				H_ASSERT(false, "Failed to get channel value. Unsupported color type.");
				return (T)0;
			}
		}

		/**
		 * Get Green Channel Value
		 * @return
		 */
		template<typename T>
		inline auto GetGreen() const
		{
			if constexpr (std::is_same_v<T, uint8_t>)
			{
				return (mColor >> 8u) & 0xFFu;
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				return ((mColor >> 8u) & 0xFFu) / 255.0f;
			}
			else
			{
				H_ASSERT(false, "Failed to get channel value. Unsupported color type.");
				return (T)0;
			}
		}

		/**
		 * Get Blue Channel Value
		 * @return
		 */
		template<typename T>
		inline auto GetBlue() const
		{
			if constexpr (std::is_same_v<T, uint8_t>)
			{
				return (mColor >> 16u) & 0xFFu;
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				return ((mColor >> 16u) & 0xFFu) / 255.0f;
			}
			else
			{
				H_ASSERT(false, "Failed to get channel value. Unsupported color type.");
				return (T)0;
			}
		}

		/**
		 * Get Alpha Channel Value
		 * @return
		 */
		template<typename T>
		inline auto GetAlpha() const 
		{
			if constexpr (std::is_same_v<T, uint8_t>)
			{
				return mColor >> 24u;
			}
			else if constexpr (std::is_same_v<T, float>)
			{
				return (mColor >> 24u) / 255.0f;
			}
			else
			{
				H_ASSERT(false, "Failed to get channel value. Unsupported color type.");
				return (T)0;
			}
		}

		/**
		 * Get Color Float4
		 * @return 
		 */
		DirectX::XMFLOAT4 GetColorFloat4() const;

		/**
		 * Get Color Float3
		 * @return 
		 */
		DirectX::XMFLOAT3 GetColorFloat3() const;

	private:
		uint32_t mColor{ 0u };

	};

}
