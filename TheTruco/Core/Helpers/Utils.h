#pragma once
#include <vector>
#include <Helpers/Constants/Constants.h>
#include <combaseapi.h>

namespace Helpers
{
    class Utils
    {
    public:
        static std::string GetCardValue(const int& cardNumber);
        static std::string GetSuitValue(const int& suitNumber);

        static std::wstring GetGameDataDirectory(const std::wstring& pathDirectory = Constants::BASE_DIRECTORY);

        static std::vector<std::string> SplitString(std::string value, const std::string& delimiter);
    };
}