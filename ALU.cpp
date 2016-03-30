#include "ALU.h"

namespace Commands { namespace ALU
{
	void add(std::string dest, uint32_t val)
	{
		mov(dest, val + Register::GetReg(dest).GetValue());
	}

	void add(std::string dest, std::string src)
	{
		add(dest, Register::GetReg(src).GetValue());
	}

	void add(OP_PARAM_TYPE args)
	{
		if(args.size() != 2)
		{
			std::cout << "\nError: add takes 2 arguments. add [reg/mem], [reg/mem/val]\n";
			return;
		}
		if(std::isdigit(args[1][0])) // is first character of the second arg a number?
			add(args[0], std::stoi(args[1]));
		else
			add(args[0], args[1]);
	}

	void sub(std::string dest, uint32_t val)
	{
		mov(dest, val - Register::GetReg(dest).GetValue());
	}

	void sub(std::string dest, std::string src)
	{
		sub(dest, Register::GetReg(src).GetValue());
	}

	void sub(OP_PARAM_TYPE args)
	{
		if(args.size() != 2)
		{
			std::cout << "\nError: sub takes 2 arguments. sub [reg/mem], [reg/mem/val]\n";
			return;
		}
		if(std::isdigit(args[1][0])) // is first character of the second arg a number?
			sub(args[0], std::stoi(args[1]));
		else
			sub(args[0], args[1]);
	}

	void Init()
	{
		ADD_OP("add", add);
		ADD_OP("sub", sub);
	}
}}
