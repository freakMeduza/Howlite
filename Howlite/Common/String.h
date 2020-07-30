#pragma once

namespace Howlite {

	struct HString {
		HString() = delete;
		~HString() = delete;

		HString(HString&&) = delete;
		HString(const HString&) = delete;
	
		HString& operator=(HString&&) = delete;
		HString& operator=(const HString&) = delete;

		/**
		 * Convert Narrow String To Wide String
		 * @param String 
		 * @return 
		 */
		static std::wstring ToWideString(const std::string& String) noexcept;

		/**
		 * Convert Wide String To Narrow String
		 * @param String 
		 * @return 
		 */
		static std::string ToNarrowString(const std::wstring& String) noexcept;
	};

}