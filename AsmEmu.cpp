#include "Register.h"
#include <algorithm>
#include <regex>
#include <vector>

// ^([a-zA-Z]+)(?: +([^,]+))?(?:, +([^,]+))?(?:, +([^,]+))?$

uint8_t verbosity = 0;

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
		throw 	"lhs(" + dest + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
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
	if(verbosity)
	{
		for (unsigned i=0; i<matches.size(); ++i) {
			std::cout << "[" << matches[i] << "] ";
		}
		std::cout << '\n';
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
			verbosity = 1;
		}
		else
		{
			std::cerr << "error: unkown argument: '" << argv[1] << "'\n";
			return 1;
		}
	}

	Register::Init();
	try
	{
		// mov("eax", 4294967295);
		// mov("ax", 300);
		mov("al", 1);
		mov("ah", "al");
		Command userInput;

		std::cout << "Welcome to TrevAssembly! Plz enter asm commands!\n";
		do
		{
			userInput = GetCommand();
			// if(opcodes[userInput.op].find)

		}while (userInput.raw != "exit");
	}
	catch (const std::string e)
	{
		std::cout << e << '\n';
	}
	std::cout << Register::GetReg("eax") << '\n';
	std::cout << Register::GetReg("ax") << '\n';
	std::cout << Register::GetReg("ah") << '\n';
	std::cout << Register::GetReg("al") << '\n';
	return 0;
}
