#include <algorithm>
#include <codecvt>
#include <locale>
#include <random>
#include <string>

#include "Color.h"
#include "Value.h"
#include "Util.h"

using namespace Uno;

// Get a random integer in the range [min, max)
int Util::randInt(int min, int max) {
	static std::random_device randomDevice;
	static std::mt19937 rng(randomDevice());

	// uniform_int_distribution bugs out when max - 1 < min

	if (min == max) {
		return min;
	}

	std::uniform_int_distribution<int> distribution(min, max - 1);
	return distribution(rng);
}

// Convert a string to a wide string
std::wstring Util::toWString(const std::string& str) {
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}