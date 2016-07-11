#pragma once

#ifndef _PATTERN_HH_
# define _PATTERN_HH_

#include <vector>

namespace Sudoku {
	struct Case;
}

namespace Pattern {
	/*
	** Main structure to stock *doku square
	*/
	struct Square {
		Square(int nbDimension);
		Square(const Square &);
		~Square();

		/*
		** Array of possibilities evaluated by occurancy in the square
		*/
		std::vector<int> _occurrencies;

		/*
		** Array of present cases in the square
		*/
		std::vector<Sudoku::Case *> _cases;

		/*
		** Bool set to the square fullness
		*/
		bool isFull;
	};

	/*
	** Main structure to stock *doku row
	*/
	struct Row {
		Row(int nbDimension);
		Row(const Row &);
		~Row();

		/*
		** Array of possibilities evaluated by occurancy in the row
		*/
		std::vector<int> _occurrencies;

		/*
		** Array of present cases in the row
		*/
		std::vector<Sudoku::Case *> _cases;

		/*
		** Bool set to the row fullness
		*/
		bool isFull;
	};

	/*
	** Main structure to stock *doku row
	*/
	struct Column {
		Column(int nbDimension);
		Column(const Column &);
		~Column();

		/*
		** Array of possibilities evaluated by occurancy in the column
		*/
		std::vector<int> _occurrencies;

		/*
		** Array of present cases in the column
		*/
		std::vector<Sudoku::Case *> _cases;

		/*
		** Bool set to the column fullness
		*/
		bool isFull;
	};
}

#endif