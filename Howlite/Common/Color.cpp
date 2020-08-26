#include "HowlitePCH.h"
#include "Color.h"

namespace Howlite {

#define COLOR(name, r, g, b) \
const HColor HColor::name (r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

	// Basic
	const HColor HColor::Transparent = HColor(0.0f, 0.0f, 0.0f, 0.0f);

	COLOR(Black, 0, 0, 0);
	COLOR(DarkGray, 25, 25, 25);
	COLOR(White, 255, 255, 255);
	COLOR(Red, 255, 0, 0);
	COLOR(Green, 0, 255, 0);
	COLOR(Blue, 0, 0, 255);
	COLOR(Yellow, 255, 255, 0);
	COLOR(Magenta, 255, 0, 255);
	COLOR(LightBlue, 0, 255, 255);
	// Red
	COLOR(DarkRed, 139, 250, 0);
	COLOR(Salmon, 250, 128, 114);
	COLOR(LightSalmon, 255, 160, 122);
	COLOR(Crimson, 220, 20, 60);
	COLOR(FireBrick, 178, 34, 34);
	// Pink
	COLOR(Pink, 255, 192, 203);
	COLOR(HotPink, 255, 105, 180);
	COLOR(DeepPink, 255, 20, 147);
	COLOR(MediumVioletRed, 199, 21, 133);
	COLOR(PaleVioletRed, 219, 112, 147);
	// Orange
	COLOR(Orange, 255, 165, 0);
	COLOR(DarkOrange, 255, 140, 0);
	COLOR(OrangeRed, 255, 69, 0);
	COLOR(Tomato, 255, 99, 71);
	COLOR(Coral, 255, 127, 80);
	// Yellow
	COLOR(Gold, 255, 215, 0);
	COLOR(LightYellow, 255, 255, 224);
	COLOR(Peach, 255, 218, 185);
	COLOR(Moccasin, 255, 228, 181);
	// Purple
	COLOR(Lavender, 230, 230, 250);
	COLOR(Violet, 238, 130, 238);
	COLOR(DarkViolet, 148, 0, 211);
	COLOR(DarkMagenta, 139, 0, 139);
	COLOR(Purple, 128, 0, 128);
	COLOR(Indigo, 75, 0, 130);
	COLOR(SlateBlue, 106, 90, 205);
	COLOR(DarkSlateBlue, 72, 61, 139);
	COLOR(MediumSlateBlue, 123, 104, 238);
	// Green
	COLOR(GreenYellow, 173, 255, 47);
	COLOR(Chartreuse, 127, 255, 0);
	COLOR(LimeGreen, 50, 205, 50);
	COLOR(PaleGreen, 152, 251, 152);
	COLOR(MediumSpringGreen, 0, 250, 154);
	COLOR(SpringGreen, 0, 255, 127);
	COLOR(SeaGreen, 46, 139, 87);
	COLOR(ForestGreen, 34, 139, 34);
	COLOR(DarkGreen, 0, 100, 0);
	COLOR(YellowGreen, 154, 205, 50);
	COLOR(OliveDrab, 107, 142, 34);
	COLOR(Olive, 128, 128, 0);
	COLOR(MediumAquamarine, 102, 205, 170);
	COLOR(LightSeaGreen, 32, 178, 170);
	COLOR(DarkCyan, 0, 139, 139);
	COLOR(Teal, 0, 128, 128);
	// Blue
	COLOR(LightCyan, 225, 255, 255);
	COLOR(Aquamarine, 127, 255, 212);
	COLOR(Turquoise, 64, 224, 208);
	COLOR(CadetBlue, 95, 158, 160);
	COLOR(SteelBlue, 70, 130, 180);
	COLOR(LightSteelBlue, 176, 196, 222);
	COLOR(ClearWater, 173, 216, 239);
	COLOR(SkyBlue, 135, 206, 235);
	COLOR(LightSkyBlue, 135, 206, 250);
	COLOR(DeepSkyBlue, 0, 191, 255);
	COLOR(DodgerBlue, 30, 144, 255);
	COLOR(RoyalBlue, 65, 105, 225);
	COLOR(MediumBlue, 0, 0, 205);
	COLOR(DarkBlue, 0, 0, 139);
	COLOR(Navy, 0, 0, 128);
	COLOR(MidnightBlue, 25, 25, 112);
	COLOR(PhyreBlue, 0, 32, 76);
	// Brown
	COLOR(CornSilk, 255, 248, 220);
	COLOR(Bisque, 255, 228, 196);
	COLOR(Wheat, 245, 222, 179);
	COLOR(BurlyWood, 222, 184, 135);
	COLOR(Tan, 210, 180, 140);
	COLOR(RosyBrown, 188, 143, 143);
	COLOR(SandyBrown, 244, 164, 96);
	COLOR(GoldenRod, 218, 165, 32);
	COLOR(Peru, 205, 133, 63);
	COLOR(SaddleBrown, 139, 69, 19);
	COLOR(Sienna, 160, 82, 45);
	COLOR(Brown, 165, 82, 45);
	COLOR(Maroon, 128, 0, 0);

	HColor::HColor(uint32_t Color) :
		mColor{ Color }
	{

	}

	HColor::HColor(const DirectX::XMFLOAT3& Color) :
		HColor{ Color.x, Color.y, Color.z }
	{

	}

	HColor::HColor(const DirectX::XMFLOAT4& Color) :
		HColor{ Color.x, Color.y, Color.z, Color.w }
	{

	}

	HColor::HColor(float RedChannel, float GreenChannel, float BlueChannel) :
		HColor{ RedChannel, GreenChannel, BlueChannel, 1.0f }
	{

	}

	HColor::HColor(float RedChannel, float GreenChannel, float BlueChannel, float AlphaChannel)
	{
		uint8_t r = static_cast<uint8_t>(RedChannel * 255.0f);
		uint8_t g = static_cast<uint8_t>(GreenChannel * 255.0f);
		uint8_t b = static_cast<uint8_t>(BlueChannel * 255.0f);
		uint8_t a = static_cast<uint8_t>(AlphaChannel * 255.0f);

		mColor = a << 24 | b << 16 | g << 8 | r;
	}

	HColor::HColor(uint8_t RedChannel, uint8_t GreenChannel, uint8_t BlueChannel) :
		HColor{ RedChannel, GreenChannel, BlueChannel, 255 }
	{

	}

	HColor::HColor(uint8_t RedChannel, uint8_t GreenChannel, uint8_t BlueChannel, uint8_t AlphaChannel)
	{
		mColor = AlphaChannel << 24 | BlueChannel << 16 | GreenChannel << 8 | RedChannel;
	}

	DirectX::XMFLOAT4 HColor::GetColorFloat4() const
	{
		using namespace DirectX;

		const float r = GetRed<float>();
		const float g = GetGreen<float>();
		const float b = GetBlue<float>();
		const float a = GetAlpha<float>();

		return XMFLOAT4{ r, g, b, a };
	}

	DirectX::XMFLOAT3 HColor::GetColorFloat3() const
	{
		using namespace DirectX;

		const float r = GetRed<float>();
		const float g = GetGreen<float>();
		const float b = GetBlue<float>();

		return XMFLOAT3{ r, g, b };
	}

}