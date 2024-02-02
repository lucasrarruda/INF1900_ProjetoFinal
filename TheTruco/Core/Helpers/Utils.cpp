#include "pch.h"
#include <Windows.h>
#include <Helpers/Utils.h>
#include <Helpers/Enums/CardsValueEnum.h>
#include <Helpers/Enums/SuitsEnum.h>
#include "Constants/CardsValueConstants.h"
#include "Constants/SuitsConstants.h"
#include <filesystem>
#include <stack>
#include <stringapiset.h>
#include <sstream>
#include <guiddef.h>

using namespace Helpers::Enums;
using namespace Helpers::CardsValueConstants;
using namespace Helpers::SuitsConstants;
using namespace Helpers;
using namespace std;

string Utils::GetCardValue(const int& cardNumber)
{
    switch (cardNumber) 
    {
        case static_cast<int>(CardsValueEnum::FOUR):
            return FOUR;
            break;
        case static_cast<int>(CardsValueEnum::FIVE):
            return FIVE;
            break;
        case static_cast<int>(CardsValueEnum::SIX):
            return SIX;
            break;
        case static_cast<int>(CardsValueEnum::SEVEN):
            return SEVEN;
            break;
        case static_cast<int>(CardsValueEnum::Q):
            return Q;
            break;
        case static_cast<int>(CardsValueEnum::J):
            return J;
            break;
        case static_cast<int>(CardsValueEnum::K):
            return K;
            break;
        case static_cast<int>(CardsValueEnum::A):
            return A;
            break;
        case static_cast<int>(CardsValueEnum::TWO):
            return TWO;
            break;
        case static_cast<int>(CardsValueEnum::THREE):
            return THREE;
            break;
        default:
            return "";
    }
}

string Utils::GetSuitValue(const int& suitNumber)
{
    switch (suitNumber)
    {
        case static_cast<int>(SuitsEnum::DIAMONDS):
            return DIAMONDS;
            break;
        case static_cast<int>(SuitsEnum::SPADES):
            return SPADES;
            break;
        case static_cast<int>(SuitsEnum::HEARTS):
            return HEARTS;
            break;
        case static_cast<int>(SuitsEnum::CLUBS):
            return CLUBS;
            break;
        default:
            return "";
    }
}

wstring Utils::GetGameDataDirectory(const std::wstring& pathDirectory)
{
    std::filesystem::path directory = pathDirectory;

    if (!std::filesystem::exists(directory))
    {
        stack<std::filesystem::path> stackPath;
        stackPath.push(directory);

        auto temporaryDirectory = directory.parent_path();
        while (!std::filesystem::exists(temporaryDirectory) && temporaryDirectory.has_parent_path())
        {
            stackPath.push(temporaryDirectory);
            temporaryDirectory = temporaryDirectory.parent_path();
        }

        while (!stackPath.empty())
        {
            error_code error;
            std::filesystem::create_directory(stackPath.top(), error);
            stackPath.pop();
        }
    }

    return directory.c_str();
}

std::vector<std::string> Utils::SplitString(std::string value, const std::string& delimiter)
{
    int end = value.find(delimiter);
    vector<string> result;
    while (end != -1) 
    {
        result.push_back(value.substr(0, end));
        value.erase(value.begin(), value.begin() + end + 1);
        end = value.find(delimiter);
    }
    result.push_back(value.substr(0, end));

    return result;
}
