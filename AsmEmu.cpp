#include "Register.h"
#include <algorithm>
#include <regex>
#include <vector>
#include <cctype>

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

void mov(std::string dest, uint32_t val)
{
	Register& reg = Register::GetReg(dest);
	if(reg.GetSize() < GetBytesNeeded(val))
		std::cout << 	"lhs(" + dest + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
						"lhs and rhs are of size " + std::to_string(reg.GetSize()) + " and " + std::to_string(GetBytesNeeded(val)) + " bytes respecively.";
	reg.SetVal(val);
}

void mov(std::string dest, std::string src)
{
	mov(dest, Register::GetReg(src).GetValue());
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

int main(int argc, char* argv[])
{
	opcodes["mov"].push_back(0x88);
	opcodes["mov"].push_back(0x89);
	opcodes["mov"].push_back(0x8A);
	opcodes["mov"].push_back(0x8B);
	opcodes["mov"].push_back(0x8C);
	opcodes["mov"].push_back(0x8E);

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
		if(userInput.op == "mov")
		{
			if(userInput.args.size() != 2)
			{
				std::cout << "\nError: mov takes 2 arguments. mov [reg/mem], [reg/mem/val]\n";
				continue;
			}
			if(std::isdigit(userInput.args[1][0])) // is first character a number?
				mov(userInput.args[0], std::stoi(userInput.args[1]));
			else
				mov(userInput.args[0], userInput.args[1]);
		}
		else if(userInput.op == "dump")
		{
			std::cout << Register::GetReg("eax") << '\n';
			std::cout << Register::GetReg("ax") << '\n';
			std::cout << Register::GetReg("ah") << '\n';
			std::cout << Register::GetReg("al") << '\n';
		}

	}while (userInput.raw != "exit");

	return 0;
}
