#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Color.h"
#include "Value.h"

namespace Uno::Util {
	int randInt(int min, int max);

	// Fisher Yates shuffle
	template <typename T>
	void shuffle(std::vector<T>& vec) {
		for (int i = vec.size() - 1; i > 0; i--) {
			std::swap(vec[i], vec[randInt(0, i + 1)]);
		}
	}

	// Remove leading whitespace
	inline void ltrim(std::string& str) {
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c) {
			return !std::isspace(c);
		}));
	};

	// Remove trailing whitespace
	inline void rtrim(std::string& str) {
		str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char c) {
			return !std::isspace(c);
		}).base(), str.end());
	}

	// Remove leading and trailing whitespace
	inline void trim(std::string& str) {
		ltrim(str);
		rtrim(str);
	}

	// Remove leading whitespace and return a copy
	inline std::string ltrim_copy(std::string str) {
		ltrim(str);
		return str;
	}

	// Remove trailing whitespace and return a copy
	inline std::string rtrim_copy(std::string str) {
		rtrim(str);
		return str;
	}

	// Remove leading and trailing whitespace and return a copy
	inline std::string trim_copy(std::string str) {
		trim(str);
		return str;
	}

	// Convert a string to lowercase
	inline void toLower(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	// Convert a string to uppercase
	inline void toUpper(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}

	// Convert a string to lowercase and return a copy
	inline std::string toLower_copy(std::string str) {
		toLower(str);
		return str;
	}

	// Convert a string to uppercase and return a copy
	inline std::string toUpper_copy(std::string str) {
		toUpper(str);
		return str;
	}

	std::wstring toWString(const std::string& str);

	// Get the ANSI color code for a color
	inline std::string getColorCode(Color color) {
		switch (color) {
			case Color::RED: return "\033[1;31m";
			case Color::BLUE: return "\033[1;34m";
			case Color::GREEN: return "\033[1;32m";
			case Color::YELLOW: return "\033[1;33m";
			case Color::WILD: return "\033[1;37m";
			default: return "";
		}
	}

	// Get the string representation of a color
	inline std::string toString(Color color) {
		switch (color) {
			case Color::RED: return "RED";
			case Color::BLUE: return "BLUE";
			case Color::GREEN: return "GREEN";
			case Color::YELLOW: return "YELLOW";
			case Color::WILD: return "WILD";
			default: return "UNKNOWN";
		}
	}

	// Get the string representation of a value
	inline std::string toString(Value value) {
		switch (value) {
			case Value::ZERO: return "0";
			case Value::ONE: return "1";
			case Value::TWO: return "2";
			case Value::THREE: return "3";
			case Value::FOUR: return "4";
			case Value::FIVE: return "5";
			case Value::SIX: return "6";
			case Value::SEVEN: return "7";
			case Value::EIGHT: return "8";
			case Value::NINE: return "9";
			case Value::REVERSE: return "REVERSE";
			case Value::SKIP: return "SKIP";
			case Value::DRAW_TWO: return "DRAW_TWO";
			case Value::WILD: return "WILD";
			case Value::WILD_DRAW_FOUR: return "WILD_DRAW_FOUR";
			default: return "UNKNOWN";
		}
	}
}