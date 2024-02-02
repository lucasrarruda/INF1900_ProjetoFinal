#pragma once
#include <string>

namespace Helpers::Constants
{
	constexpr const unsigned char BYTE_ORDER_MARK_SETTINGS[4] = { 0xFF, 0xFE, 0xBB, 0xAA };
	constexpr const unsigned char BYTE_ORDER_MARK_UNICODE[2] = { 0xFF, 0xFE };
	constexpr const unsigned char BYTE_ORDER_MARK_UNICODE_BE[2] = { 0xFE, 0xFF };
	constexpr const unsigned char BYTE_ORDER_MARK_UTF8[2] = { 0xEF, 0xBB };

	constexpr const char* UNICODE_DEFAULT_BUFFER = "\xFF\xFE";
	constexpr const char* UNICODE_BIG_ENDIAN_BUFFER = "\xFE\xFF";
	constexpr const char* UTF8_WITH_BYTE_ORDER_MARK_BUFFER = "\xEF\xBB\xBF";
	constexpr const char* TRUCO_BUFFER = "\xFF\xFE\xBB\xAA";

	constexpr const int NUMBER_BYTES_WRITE_ZERO = 0;
	constexpr const int NUMBER_BYTES_WRITE_TWO = 2;
	constexpr const int NUMBER_BYTES_WRITE_THREE = 3;
	constexpr const int NUMBER_BYTES_WRITE_FOUR = 4;

	constexpr const wchar_t* NULL_STR = L"null";
	constexpr const wchar_t* EMPTY = L"";
	constexpr const wchar_t* DATABASE_FILE = L"\\TrucoGameDatabase.ini";
	constexpr const wchar_t* BASE_DIRECTORY = LR"(C:\ProgramData\TrucoGame)";

	const std::string DATABASE_USERS = "Users";
	const std::string DATABASE_GAMES = "Games";
}