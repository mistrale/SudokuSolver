#pragma once

#ifndef _SUDOKU_HH_
# define _SUDOKU_HH_

#include "Pattern.hh"

#include <vector>
#include <map>
#include <stack>

# define UNASSIGNED '.' - '0'

namespace Sudoku
{
	/*
	** Main structure for *doku cases stocking
	*/
	struct Case {
		Case(int _line, int _col, int assignedValue);
		bool operator<(const Case &point) const;

		/*
		** Case coordinate by line 
		*/
		int line;

		/*
		** Case coordinate by column
		*/
		int col;

		/*
		** Case value : 
		** - UNASSIGNED if the number is not assigned yet
		** - 1 <= value <= n for n = map dimension
		*/
		int value;

		/*
		** Factors array containing all the case possibilities 
		*/
		std::vector<int> _factors;
	};

	/*
	** Main class to solve *doku
	*/
	class Engine
	{
	public:
		static void removeoccurancy(int &);

		bool init(const std::string & file);
		bool resolve();
		
		void updateLineFactor(const Case * const point);
		void updateColumnFactor(const Case * const point);
		void updateSquareFactor(const Case * const point);
		bool checkFactor();
		void displayMap();

		void setDimension(int nb);

		std::vector<Pattern::Column> &getColumns();
		std::vector<Pattern::Row> &getRows();
		std::vector<Pattern::Square> &getSquares();

		std::vector<Case *>	&getMap();
		std::stack<Case *>	&getExistingCase();

		size_t getDimension() const;
		size_t getSide() const;


	private:
		/*
		** Sudoku map containing each case with informations
		*/
		std::vector<Case *>	_map;

		/*
		** Stack containing each assigned case (new or already existing)  
		*/
		std::stack<Case *>	_existingCase;

		/*
		** Array containing all possible columns
		*/
		std::vector<Pattern::Column> _columns;

		/*
		** Array containing all possible rows
		*/
		std::vector<Pattern::Row> _rows;

		/*
		** Array containing all possible squares
		*/
		std::vector<Pattern::Square> _squares;

		/*
		** Dimension of the *doku
		*/
		size_t				_mapDimension;

		/*
		** *doku side edge
		*/
		size_t				_nbSide;

		/*
		** *doku size case
		*/
		size_t				_mapSize;
	};
}

#endif