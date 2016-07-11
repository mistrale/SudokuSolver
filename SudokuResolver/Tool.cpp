#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

#include "Tool.hh"

namespace Tool {
	/*
	** Parse the file 'file' passed as parameter and initiliaze the *doku map.
	**
	** Parse each line and each column of the file. Create a new Case for each
	** iteration. If the case value is already assigned to a number, push the case
	** to engine existing cases. Otherwise, initiliaze its factors array to all
	** possibilities depending on *doku dimension. In any case, push the case
	** to the *doku map, its corresponding square, row and column.
	**
	** @param 'file' file containing the *doku map.
	** @param 'engine' engine which will solve the *doku that need to be initialized.
	**
	** return initialization success.
	*/
	bool Parser::parseFile(const std::string &file, Sudoku::Engine *engine) {
		std::ifstream infile;
		std::string line;
		int startLine = 0;
		if (!infile) {
			std::cerr << "File '" << file << "' does not exist." << std::endl;
			return false;
		}
		infile.open(file, std::ios::in);

		std::vector<Sudoku::Case *> &map = engine->getMap();
		std::stack<Sudoku::Case *> &existingCase = engine->getExistingCase();
		std::vector<Pattern::Column> &columns = engine->getColumns();
		std::vector<Pattern::Row> &rows = engine->getRows();
		std::vector<Pattern::Square> &squares = engine->getSquares();

		while (std::getline(infile, line)) {
			if (startLine == 0) {
				engine->setDimension(line.size());
			}
			for (int i = 0; i < line.size(); i++) {
				int lineSquare = (startLine / engine->getDimension());
				int colSquare = (i / engine->getDimension());
				Sudoku::Case * newCase = new Sudoku::Case(startLine, i, line[i] - '0');

				if (line[i] == '.') {
					newCase->_factors = std::vector<int>(line.size());
					std::iota(std::begin(newCase->_factors), std::end(newCase->_factors), 1);
				}
				else {
					// j'ai trouvé une case assignée
					existingCase.push(newCase);
				}
				map.push_back(newCase);

				columns[i]._cases.push_back(newCase);

				rows[startLine]._cases.push_back(newCase);
				squares[lineSquare * engine->getDimension() + colSquare]._cases.push_back(newCase);
			}
			startLine++;
		}
		return true;
	}
}