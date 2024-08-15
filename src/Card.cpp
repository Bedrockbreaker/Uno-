#include <iostream>
#include <string>

#include "Card.h"
#include "Util.h"

using namespace Uno;

// Standard constructor
Card::Card(Color color, Value value) : color(color), value(value) {}

// getter color
Color Card::getColor() const {
	return color;
}

// getter value
Value Card::getValue() const {
	return value;
}

// setter color
void Card::setColor(Color color) {
	this->color = color;
}

// getter display color
char Card::getDisplayColor() const {
	switch (color) {
		case Color::RED:
			return 'R';
		case Color::BLUE:
			return 'B';
		case Color::GREEN:
			return 'G';
		case Color::YELLOW:
			return 'Y';
		case Color::WILD:
			return 'W';
		default:
			return 'U';
	}
}

// getter display value
std::string Card::getDisplayValue() const {
	switch (value) {
		case Value::ZERO:
		case Value::ONE:
		case Value::TWO:
		case Value::THREE:
		case Value::FOUR:
		case Value::FIVE:
		case Value::SIX:
		case Value::SEVEN:
		case Value::EIGHT:
		case Value::NINE:
		case Value::REVERSE:
		case Value::SKIP:
			return getSymbol() + " ";
		case Value::DRAW_TWO:
			return "D2";
		case Value::WILD:
			return "  ";
		case Value::WILD_DRAW_FOUR:
			return "D4";
		default:
			return "U ";
	}
}

// getter symbol
std::string Card::getSymbol() const {
	switch (value) {
		case Value::ZERO:
			return "0";
		case Value::ONE:
			return "1";
		case Value::TWO:
			return "2";
		case Value::THREE:
			return "3";
		case Value::FOUR:
			return "4";
		case Value::FIVE:
			return "5";
		case Value::SIX:
			return "6";
		case Value::SEVEN:
			return "7";
		case Value::EIGHT:
			return "8";
		case Value::NINE:
			return "9";
		case Value::REVERSE:
			return "R";
		case Value::SKIP:
			return "S";
		case Value::DRAW_TWO:
			return "D2";
		case Value::WILD:
			return "";
		case Value::WILD_DRAW_FOUR:
			return "D4";
		default:
			return "U";
	}
}

// operator ==
bool Card::operator==(const Card& other) const {
	return color == other.color && value == other.value;
}

// operator !=
bool Card::operator!=(const Card& other) const {
	return !(*this == other);
}

// operator <<
namespace Uno {
	std::wostream& operator<<(std::wostream& out, const Card& card) {
		return out
			<< "Card{"
			<< card.getDisplayColor()
			<< Util::toWString(card.getSymbol())
			<< "}";
	}
}