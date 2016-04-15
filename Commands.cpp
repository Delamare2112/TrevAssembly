#include "Commands.h"
#include <cctype>
#include <regex>

size_t GetBytesNeeded(uint32_t val)
{
	if(val == (uint8_t)val)
		return 1;
	if(val == (uint16_t)val)
		return 2;
	if(val == (uint32_t)val)
		return 4;
	return 0;
}

std::vector<std::string> SearchMaches(std::string str, std::string regexStr)
{
	std::smatch matches;
	std::regex regex(regexStr);
	std::vector<std::string> retVector;
	while(std::regex_search(str, matches, regex))
	{
		std::string arg = std::string(matches[0].first, matches[0].second);
		if(arg[0] == '\"')
			for(int i = 1; i < arg.length() - 1; i++)
				retVector.push_back(std::string(1, arg[i]));
		else
			retVector.push_back(arg);
		str = matches.suffix().str();
	}
	return retVector;
}

namespace Commands
{
	Command Command::currentCommand;
	std::unordered_map<std::string, std::function<void(OP_PARAM_TYPE)>> ops;
	std::vector<Command> commandHistory;

	Command GetCommand()
	{
		Command retCommand;
		std::cout << "-->";
		std::getline(std::cin , retCommand.raw);
		std::smatch matches;
		// Command parse
		std::regex_match(retCommand.raw, matches, std::regex("^([a-zA-Z]+)(?: )?(?:[ ,]+)?(.+)?"));
		// for(auto i : matches)
		// 	std::cout << "arg: " << i << '\n';
		retCommand.op = matches[1];

		// Argument parse
		// std::cout << "\nnow doing: " << std::string(matches[2]) << '\n';
		retCommand.args = SearchMaches(matches[2], "([^, ]+)");

		// mem parse
		if(Commands::ops.find(retCommand.op) == Commands::ops.end())
		{
			// std::cout << retCommand.op << " not found, attempting memcreate syntax\n";
			retCommand.args.clear();
			std::regex_match(retCommand.raw, matches, std::regex("^([a-zA-Z]+) ([a-zA-Z]+) (.+)"));
			// for(auto i : matches)
			// 	std::cout << "arg: " << i << '\n';
			retCommand.op = matches[2];
			retCommand.args.push_back(matches[1]);

			// Argument parse
			std::string strArgs = matches[3];
			// std::cout << "\nnow doing: " << strArgs << '\n';
			retCommand.args = SearchMaches(matches[3], "([^, ]+)");
			// for(std::string s : retCommand.args)
			// 	std::cout << s << '\n';
		}

		commandHistory.push_back(retCommand);
		return retCommand;
	}

	void Command::Execute()
	{
		auto opItter = Commands::ops.find(op);
		if(opItter != Commands::ops.end())
			opItter->second(args);
		else
			std::cout << op << " not found.\n";
	}

	void mov(Register& dest, uint32_t val)
	{
		if(dest.GetSize() < GetBytesNeeded(val))
			std::cout << 	"lhs(" + Command::currentCommand.args[0] + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
							"lhs and rhs are of size " + std::to_string(dest.GetSize()) + " and " + std::to_string(GetBytesNeeded(val)) + " bytes respecively.\n";
		else
			dest.SetValue(val);
	}

	void mov(std::string dest, uint32_t val)
	{
		mov(Register::GetReg(dest), val);
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
		std::cout << Register::flags << '\n';
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
