#include "HowlitePCH.h"
#include "UUID.h"

namespace Howlite {

	std::random_device HUUID::mDevice = std::random_device{};
	std::mt19937 HUUID::mGenerator = std::mt19937{mDevice()};
	std::uniform_int_distribution<> HUUID::mDistribution1 = std::uniform_int_distribution<>(0, 15);
	std::uniform_int_distribution<> HUUID::mDistribution2 = std::uniform_int_distribution<>(8, 11);

	std::string HUUID::GenerateUUID() noexcept
	{
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
        {
            ss << mDistribution1(mGenerator);
        }
        ss << "-";
        for (i = 0; i < 4; i++)
        {
            ss << mDistribution1(mGenerator);
        }
        ss << "-4";
        for (i = 0; i < 3; i++)
        {
            ss << mDistribution1(mGenerator);
        }
        ss << "-";
        ss << mDistribution2(mGenerator);
        for (i = 0; i < 3; i++)
        {
            ss << mDistribution1(mGenerator);
        }
        ss << "-";
        for (i = 0; i < 12; i++)
        {
            ss << mDistribution1(mGenerator);
        };
        return ss.str();
	}

}