#pragma once

#include <iostream>
#include <string>

#include "Color.h"
#include "Value.h"

namespace Uno {
	class Card {

	private:
		Uno::Color color;
		Uno::Value value;

	public:
		Card(Uno::Color color, Uno::Value value);

		Uno::Color getColor() const;
		Uno::Value getValue() const;

		void setColor(Uno::Color color);

		char getDisplayColor() const;
		std::string getDisplayValue() const;
		std::string getSymbol() const;

		bool operator==(const Card& other) const;
		bool operator!=(const Card& other) const;
		friend std::wostream& operator<<(std::wostream& out, const Card& card);
	};
}