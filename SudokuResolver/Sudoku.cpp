#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>

#include "Sudoku.hh" 
#include "Tool.hh"

namespace Sudoku {
	Case::Case(int l, int r, int assignedValue) : line(l), col(r), value(assignedValue), _factors({}) {
		if (assignedValue != UNASSIGNED && assignedValue >= 10)
			value -= 7;
	}

	bool Case::operator<(const Case &point) const {
		if (line < point.line)
			return true;
		if (line > point.line)
			return false;
		return col < point.col;
	}

	void Engine::removeoccurancy(int & nb) {
		if (nb > 1)
			nb--;
	}

	bool Engine::init(const std::string &file) {
		if (!Tool::Parser::parseFile(file, this)) {
			std::cerr << "Couldn't initialize sudoku." << std::endl;
			return false;
		}
		return true;
	}

	void Engine::setDimension(int nb) {
		_nbSide = nb;
		_mapDimension = sqrt(nb);
		_mapSize = nb * nb;
		_columns = std::vector<Pattern::Column>(_nbSide, Pattern::Column(_mapDimension));
		_rows = std::vector<Pattern::Row>(_nbSide, Pattern::Row(_mapDimension));
		_squares = std::vector<Pattern::Square>(_nbSide, Pattern::Square(_mapDimension));
	}

	void Engine::displayMap() {
		for (int i = 0; i < _map.size(); ++i) {
			if (i % _nbSide == 0 && i / _nbSide >= 1)
				printf("\n");
			if (_map[i]->value == UNASSIGNED)
				printf(".");
			else {
				if (_map[i]->value > 9)
					printf("%c", _map[i]->value + 7 + '0');
				else
					printf("%d", _map[i]->value);
			}
		}
		printf("\n\n");
	}


	/*
	** Update factors of each case set on the same column as the current case.
	**
	** Iterate on each case set on the current case column. If the case value on
	** which it iterates throught is unassigned, search its factor equal to the current
	** case value and remove it. If there is only one factor in the array of the
	** iterated case, set the case value to the factor value and redo the same
	** operation on the new assigned case. 
	**
	** @param 'point' current case containing the factor value to be removed in column
	** cases.
	**
	*/
	void Engine::updateColumnFactor(const Case * const point) {

		for (int i = point->col; i < _mapSize; i += _nbSide) {
			if (i != (point->col + _nbSide * point->line)) {
				Case *current = _map[i];

				if (current->value == UNASSIGNED) {
					std::vector<int> &vec = current->_factors;
					auto find = std::find(vec.begin(), vec.end(), point->value);
					if (find != vec.end()) {
						vec.erase(find);
					}
					if (vec.size() == 1) {
						current->value = vec[0];
						updateColumnFactor(current);
						updateLineFactor(current);
						updateSquareFactor(current);
						vec.clear();
					}
				}
			}
		}
	}

	/*
	** Update factors of each case set on the same row as the current case.
	**
	** Iterate on each case set on the current case row. If the case value on
	** which it iterates throught is unassigned, search its factor equal to the current
	** case value and remove it. If there is only one factor in the array of the
	** iterated case, set the case value to the factor value and redo the same
	** operation on the new assigned case.
	**
	** @param 'point' current case containing the factor value to be removed in row
	** cases.
	**
	*/
	void Engine::updateLineFactor(const Case * const point) {
  		int start = point->line * _nbSide;
		for (int i = start; i < start + _nbSide; i++) {
			if (i != (point->col + _nbSide * point->line)) {
				Case *current = _map[i];

				if (current->value == UNASSIGNED) {
					std::vector<int> &vec = current->_factors;
					auto find = std::find(vec.begin(), vec.end(), point->value);
					if (find != vec.end()) {
						vec.erase(find);
					}
					if (vec.size() == 1) {
						current->value = vec[0];
						updateColumnFactor(current);
						updateLineFactor(current);
						updateSquareFactor(current);
						vec.clear();
					}
				}
			}
		}
	}

	/*
	** Update factors of each case set on the same square as the current case.
	**
	** Iterate on each case set on the current case square. If the case value on
	** which it iterates throught is unassigned, search its factor equal to the current
	** case value and remove it. If there is only one factor in the array of the
	** iterated case, set the case value to the factor value and redo the same
	** operation on the new assigned case.
	**
	** @param 'point' current case containing the factor value to be removed in square
	** cases.
	**
	*/
	void Engine::updateSquareFactor(const Case *point) {
		int lineSquare = (point->line / _mapDimension) * _mapDimension;
		int colSquare = (point->col / _mapDimension) * _mapDimension;
		int stopLineSquare = lineSquare + _mapDimension;
		int stopColSquare = colSquare + _mapDimension;

		while (lineSquare < stopLineSquare) {
			colSquare = (point->col / _mapDimension) * _mapDimension;
			while (colSquare < stopColSquare) {
				if (!(lineSquare == point->line && colSquare == point->col)) {
					int index = lineSquare * _nbSide + colSquare;
					Case *current = _map[index];

					if (current->value == UNASSIGNED) {
						std::vector<int> &vec = current->_factors;
						auto find = std::find(vec.begin(), vec.end(), point->value);
						if (find != vec.end()) {
							vec.erase(find);
						}
						if (vec.size() == 1) {
							current->value = vec[0];
							updateColumnFactor(current);
							updateLineFactor(current);
							updateSquareFactor(current);
							vec.clear();
						}
					}
				}
				colSquare++;
			}
			lineSquare++;
		}
	}

	/*
	** Check if we can deduce an assigned case value.
	**
	** Iterate on all map cases. If the current case value is set to UNASSIGNED
	** then iterate on all its possible factors and update the factor occurency of
	** its associated square, row and column. Else set the factor occurency of its
	** its associated square, row and column to 0. Then iterate on each existing
	** square, row and column in the map and check theirs occurencies array. If there
	** is only one factor occurency then find the unassigned case containing the current
	** factor and set its value equal to the factor and push the case to exisitingCase
	** stack.
	**
	** @param 'update' true if the function deduce at least one new assigned case.
	**
	*/
	bool Engine::checkFactor() {
		bool update = false;

		for (int i = 0; i < _map.size(); i++) {
			Case *current = _map[i];
			int idSquare = (current->line / _mapDimension) * _mapDimension  + current->col / _mapDimension;

			if (current->value == UNASSIGNED) {
				std::vector<int> &vec = current->_factors;

				for (int itFactor = 0; itFactor < vec.size(); itFactor++) {
					_columns[current->col]._occurrencies[vec[itFactor] - 1] += 1;
					_rows[current->line]._occurrencies[vec[itFactor] - 1] += 1;
					_squares[idSquare]._occurrencies[vec[itFactor] - 1] += 1;
				}
			}
			else {
				_columns[current->col]._occurrencies[current->value - 1] = 0;
				_rows[current->line]._occurrencies[current->value - 1] = 0;
				_squares[idSquare]._occurrencies[current->value -1] = 0;
			}
	
		}
		for (int i = 0; i < _nbSide; i++) {

			for (int j = 0; j < _nbSide; j++) {
				int value = j + 1;
				if (_squares[i]._occurrencies[j] == 1) {
					auto find = std::find_if(_squares[i]._cases.begin(), _squares[i]._cases.end(), [&value](const Case * tmp) -> bool {
						return std::find(tmp->_factors.begin(), tmp->_factors.end(), value) != tmp->_factors.end();
					});
					if (find != _squares[i]._cases.end()) {
						(*find)->value = value;
						(*find)->_factors.clear();
						_existingCase.push(*find);
						update = true;
					}
				}
				if (_rows[i]._occurrencies[j] == 1) {
					int value = j + 1;
					auto find = std::find_if(_rows[i]._cases.begin(), _rows[i]._cases.end(), [&value](const Case * tmp) -> bool {
						return std::find(tmp->_factors.begin(), tmp->_factors.end(), value) != tmp->_factors.end();
					});
					if (find != _rows[i]._cases.end()) {
						(*find)->value = value;
						(*find)->_factors.clear();

						_existingCase.push(*find);
						update = true;
					}
				}
				if (_columns[i]._occurrencies[j] == 1) {

					int value = j + 1;
					auto find = std::find_if(_columns[i]._cases.begin(), _columns[i]._cases.end(), [&value](const Case * tmp) -> bool {
						return std::find(tmp->_factors.begin(), tmp->_factors.end(), value) != tmp->_factors.end();
					});
					if (find != _columns[i]._cases.end()) {
						(*find)->value = value;
						(*find)->_factors.clear();
						_existingCase.push(*find);
						update = true;
					}
				}
			}

		}
		std::for_each(_columns.begin(), _columns.end(), [this](Pattern::Column &tmp) {
			tmp._occurrencies = std::vector<int>(this->getSide(), 0);
		});
		std::for_each(_rows.begin(), _rows.end(), [this](Pattern::Row &tmp) {
			tmp._occurrencies = std::vector<int>(this->getSide(), 0);
		});
		std::for_each(_squares.begin(), _squares.end(), [this](Pattern::Square &tmp) {
			tmp._occurrencies = std::vector<int>(this->getSide(), 0);
		});
		return update;
	}

	/*
	** Recursive function to solve *doku.
	**
	** While there are assigned cases, update the current row, square and column
	** associated to each assigned case. Remove the assigned case from existingCase
	** stack. Check if we can deduce an assigned case value. If there are new assigned
	** cases then recursively call itself.
	**
	** @return *doku successful resolution 
	*/
	bool Engine::resolve() {
		while (!_existingCase.empty()) {
			Case *current = _existingCase.top();
			updateColumnFactor(current);
			updateLineFactor(current);
			updateSquareFactor(current);
			_existingCase.pop();
		}
		bool success = checkFactor();
		if (!_existingCase.empty() || success) {
			return resolve();
		}
		return (!success && !_existingCase.empty());
	}

	std::vector<Pattern::Row> &Engine::getRows() {
		return _rows;
	}

	std::vector<Pattern::Column> &Engine::getColumns() {
		return _columns;
	}

	std::vector<Pattern::Square> &Engine::getSquares() {
		return _squares;
	}

	std::vector<Case *> &Engine::getMap() {
		return _map;
	}

	std::stack<Case *> &Engine::getExistingCase() {
		return _existingCase;
	}

	size_t Engine::getDimension() const {
		return _mapDimension;
	}

	size_t Engine::getSide() const {
		return _nbSide;
	}
}