#pragma once

#ifndef _TOOL_HH
# define _TOOL_HH

#include "Sudoku.hh"

namespace Tool {
	class Parser
	{
	public:
		static bool parseFile(const std::string &file, Sudoku::Engine *engine);

	private:

	};
}

#endif