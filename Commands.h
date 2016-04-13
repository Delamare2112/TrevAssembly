#pragma once
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include "Register.h"

#define ADD_OP(str, name) ops[str] = (void(*)(OP_PARAM_TYPE))&name;
#define OP_PARAM_TYPE const std::vector<std::string>&

namespace Commands
{
	struct Command
	{
		static Command currentCommand;
		std::string raw;
		std::string op;
		std::vector<std::string> args;
		void Execute();
	};
	extern std::unordered_map<std::string, std::function<void(OP_PARAM_TYPE)>> ops;
	extern std::vector<Command> commandHistory;
	void Init();
	void mov(Register& dest, uint32_t val);
	void mov(std::string dest, uint32_t val);
	void mov(std::string dest, std::string src);
	void mov(OP_PARAM_TYPE args);
	void dump(OP_PARAM_TYPE args);
	void exit(OP_PARAM_TYPE args);
	Command GetCommand();
}
