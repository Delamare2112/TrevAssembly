#include "Register.h"
#include <algorithm>
#include <regex>
#include <vector>
#include <cctype>
#include <functional>

#define OP_PARAM_TYPE const std::vector<std::string>&
#define OP_PARAMS OP_PARAM_TYPE args
#define OP_RET void
#define CREATE_OP_SIG(name) OP_RET name (OP_PARAMS)

bool verbose = 0;

size_t GetBytesNeeded(uint val)
{
	if(val == (uint8_t)val)
		return 1;
	if(val == (uint16_t)val)
		return 2;
	if(val == (uint32_t)val)
		return 4;
	return 0;
}

void _mov(std::string dest, uint32_t val)
{
	Register& reg = Register::GetReg(dest);
	if(reg.GetSize() < GetBytesNeeded(val))
		std::cout << 	"lhs(" + dest + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
						"lhs and rhs are of size " + std::to_string(reg.GetSize()) + " and " + std::to_string(GetBytesNeeded(val)) + " bytes respecively.\n";
	reg.SetVal(val);
}

void _mov(std::string dest, std::string src)
{
	_mov(dest, Register::GetReg(src).GetValue());
}

// void mov(const std::vector<std::string>& args)
CREATE_OP_SIG(mov)
{
	if(args.size() != 2)
	{
		std::cout << "\nError: mov takes 2 arguments. mov [reg/mem], [reg/mem/val]\n";
		return;
	}
	if(std::isdigit(args[1][0])) // is first character a number?
		_mov(args[0], std::stoi(args[1]));
	else
		_mov(args[0], args[1]);
}

CREATE_OP_SIG(dump)
{
	std::cout << Register::GetReg("eax") << '\n';
	std::cout << Register::GetReg("ebx") << '\n';
	std::cout << Register::GetReg("ecx") << '\n';
	std::cout << Register::GetReg("edx") << '\n';
}

struct Command
{
	std::string raw;
	std::string op;
	std::vector<std::string> args;
};

Command GetCommand()
{
	Command retCommand;
	std::cout << "-->";
	std::getline(std::cin , retCommand.raw);
	std::smatch matches;
	std::regex_match(retCommand.raw, matches, std::regex("^([a-zA-Z]+)(?: +([^,]+))?(?:, +([^,]+))?(?:, +([^,]+))?$"));
	if(verbose)
	{
		for(auto& match : matches)
			std::cout << "[" << match << "]";
		std::cout << '\n';
	}
	retCommand.op = matches[1];
	for(size_t i = 2; i < matches.size(); i++)
	{
		if(matches[i] != "")
			retCommand.args.push_back(matches[i]);
	}
	return retCommand;
}

std::unordered_map<std::string, std::vector<uint8_t>> opcodes;
std::unordered_map<std::string, std::function<OP_RET(OP_PARAM_TYPE)>> ops;

int main(int argc, char* argv[])
{
	ops["0x88"] = mov;
	ops["0x89"] = mov;
	ops["0x8A"] = mov;
	ops["0x8B"] = mov;
	ops["0x8C"] = mov;
	ops["0x8E"] = mov;
	ops["mov"] = mov;
	ops["dump"] = dump;

	if(argc > 1)
	{
		if((std::string)argv[1] == "-v")
		{
			verbose = 1;
		}
		else
		{
			std::cerr << "error: unkown argument: '" << argv[1] << "'\n";
			return 1;
		}
	}

	Register::Init();
	Command userInput;

	std::cout << "Welcome to TrevAssembly! Plz enter asm commands!\n";
	do
	{
		userInput = GetCommand();
		auto opItter = ops.find(userInput.op);
		if(opItter != ops.end())
			opItter->second(userInput.args);
		else
			std::cout << userInput.op << " not found.\n";
		// if(userInput.op == "mov")
		// {
		// 	mov(userInput.args);
		// }
		// else if(userInput.op == "dump")
		// {
		// 	dump(userInput.args);
		// }

	}while (userInput.raw != "exit");

	return 0;
}
