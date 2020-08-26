#pragma once

namespace Howlite {

	struct HUUID {
		HUUID() = delete;
		~HUUID() = delete;

		HUUID(HUUID&&) = delete;
		HUUID(const HUUID&) = delete;

		HUUID& operator=(HUUID&&) = delete;
		HUUID& operator=(const HUUID&) = delete;

		/**
		 * Generate UUID
		 * @return 
		 */
		static std::string GenerateUUID() noexcept;

	private:
		static std::random_device mDevice;
		static std::mt19937 mGenerator;
		static std::uniform_int_distribution<> mDistribution1;
		static std::uniform_int_distribution<> mDistribution2;

	};

}
