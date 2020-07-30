#include "HowlitePCH.h"
#include "String.h"

namespace Howlite {

	std::wstring HString::ToWideString(const std::string& String) noexcept
	{
		wchar_t buffer[512];
		mbstowcs_s(nullptr, buffer, String.c_str(), _TRUNCATE);
		return buffer;
	}

	std::string HString::ToNarrowString(const std::wstring& String) noexcept 
	{
		char buffer[512];
		wcstombs_s(nullptr, buffer, String.c_str(), _TRUNCATE);
		return buffer;
	}

}