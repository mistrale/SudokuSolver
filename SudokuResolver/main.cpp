#include <ctime>
#include <iostream>

#include "Tool.hh"
#include "Sudoku.hh"


int	main(int ac, char **av) {
	if (av[1] == NULL) {
		std::cerr << "No given file" << std::endl;
		return -1;
	}
	Sudoku::Engine engine;
	engine.init(av[1]);
	std::cout << "Starting grid : " << std::endl << std::endl;
	engine.displayMap();

	std::clock_t start;
	double duration;
	start = std::clock();
	bool success = engine.resolve();
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout  << "Finishing grid : " << std::endl << std::endl;
	engine.displayMap();
	if (success) {
		std::cout << "THIS MAP IS TOO EVIL FOR ME" << std::endl;
	}
	else {
		std::cout << "So easy to solve." << std::endl;
	}
	std::cout  << "Elapsed time : " << duration << std::endl;
	return 0;
}