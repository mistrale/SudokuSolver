#include "Pattern.hh"

/*
** All pattern possibilities arrays are initialized to the size
** of the *doku dimension ^ 2 and set to 0.
*/

namespace Pattern {
	Square::Square(int nbDimension) : isFull(false) {
		_occurrencies = std::vector<int>(nbDimension * nbDimension, 0);
	}

	Square::Square(const Square &other) {
		_occurrencies = other._occurrencies;
		isFull = other.isFull;
	}

	Square::~Square() {
		_occurrencies.clear();
	}

	Row::Row(int nbDimension) : isFull(false) {
		_occurrencies = std::vector<int>(nbDimension * nbDimension, 0);
	}

	Row::Row(const Row &other) {
		_occurrencies = other._occurrencies;
		isFull = other.isFull;
	}

	Row::~Row() {
		_occurrencies.clear();
	}

	Column::Column(int nbDimension) : isFull(false) {
		_occurrencies = std::vector<int>(nbDimension * nbDimension, 0);
	}

	Column::Column(const Column &other) {
		_occurrencies = other._occurrencies;
		isFull = other.isFull;
	}

	Column::~Column() {
		_occurrencies.clear();
	}
}