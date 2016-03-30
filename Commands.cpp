#include "Commands.h"
#include "Register.h"
#include <cctype>
#include <regex>

#define ADD_OP(str, name) ops[str] = (void(*)(OP_PARAM_TYPE))&name;

namespace Commands
{
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

	void Command::Execute()
	{
		auto opItter = Commands::ops.find(op);
		if(opItter != Commands::ops.end())
			opItter->second(args);
		else
			std::cout << op << " not found.\n";
	}

	std::unordered_map<std::string, std::function<void(OP_PARAM_TYPE)>> ops;

	Command GetCommand()
	{
		Command retCommand;
		std::cout << "-->";
		std::getline(std::cin , retCommand.raw);
		std::smatch matches;
		std::regex_match(retCommand.raw, matches, std::regex("^([a-zA-Z]+)(?: +([^,]+))?(?:, +([^,]+))?(?:, +([^,]+))?$"));
		retCommand.op = matches[1];
		for(size_t i = 2; i < matches.size(); i++)
		{
			if(matches[i] != "")
				retCommand.args.push_back(matches[i]);
		}
		return retCommand;
	}

	void mov(std::string dest, uint32_t val)
	{
		Register& reg = Register::GetReg(dest);
		if(reg.GetSize() < GetBytesNeeded(val))
			std::cout << 	"lhs(" + dest + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
							"lhs and rhs are of size " + std::to_string(reg.GetSize()) + " and " + std::to_string(GetBytesNeeded(val)) + " bytes respecively.\n";
		reg.SetVal(val);
	}

	void mov(std::string dest, std::string src)
	{
		mov(dest, Register::GetReg(src).GetValue());
	}

	void mov(OP_PARAM_TYPE args)
	{
		if(args.size() != 2)
		{
			std::cout << "\nError: mov takes 2 arguments. mov [reg/mem], [reg/mem/val]\n";
			return;
		}
		if(std::isdigit(args[1][0])) // is first character a number?
			mov(args[0], std::stoi(args[1]));
		else
			mov(args[0], args[1]);
	}

	void dump(OP_PARAM_TYPE args)
	{
		std::cout << Register::GetReg("eax") << '\n';
		std::cout << Register::GetReg("ebx") << '\n';
		std::cout << Register::GetReg("ecx") << '\n';
		std::cout << Register::GetReg("edx") << '\n';
	}

	void exit(OP_PARAM_TYPE args)
	{
		std::cout << "Byebye now!\n";
	}

	void Init()
	{
		ADD_OP("0x88", mov);
		ADD_OP("0x89", mov);
		ADD_OP("0x8A", mov);
		ADD_OP("0x8B", mov);
		ADD_OP("0x8C", mov);
		ADD_OP("0x8E", mov);
		ADD_OP("mov", mov);
		ADD_OP("dump", dump);
		ADD_OP("exit", exit);
	}
}
