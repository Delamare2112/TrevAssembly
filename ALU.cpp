#include "ALU.h"
#include "Memory.h"
#include <cmath>

namespace Commands { namespace ALU
{
	bool CheckParity(uint8_t bits)
	{
		bool even = 1;
		for(uint8_t i = 0; i < 8; i++)
			if(bits & (1<<i))
				even = !even;
		return even;
	}

	void add(std::string dest, uint32_t val)
	{
		Register& destReg = Register::GetReg(dest);
		uint32_t sum = val + destReg.GetValue();

		// I don't just (int32_t) because size matters.
		bool destSigned = destReg.GetBit((destReg.GetSize() * 8) - 1); // is the dest reg neg when signed
		bool valSigned = val & (1 << ((destReg.GetSize() * 8) - 1));   // is the src val neg when signed
		bool sumSigned = sum & (1 << ((destReg.GetSize() * 8) - 1));   // is the sum neg when signed
		bool overflow = (destSigned && valSigned && !sumSigned) || (!destSigned && !valSigned && sumSigned);
		bool carry = (uint64_t)destReg.GetValue() + (uint64_t)val != (destReg.FitToSize(sum));

		Register::flags.SetBit(FlagMask::overflow, overflow);
		Register::flags.SetBit(FlagMask::carry, carry);
		Register::flags.SetBit(FlagMask::sign, sumSigned);
		Register::flags.SetBit(FlagMask::parity, CheckParity(sum));
		Register::flags.SetBit(FlagMask::zero, sum == 0);

		mov(destReg, destReg.FitToSize(sum));
	}

	void add(std::string dest, std::string src)
	{
		Register& reg = Register::GetReg(src);
		if(reg.GetSize() != 0)
			add(dest, reg.GetValue());
		else
			add(dest, Memory::GetMem<uint32_t>(Memory::labels[src]));
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
		// mov(dest, val - Register::GetReg(dest).GetValue());
		Register& destReg = Register::GetReg(dest);
		uint32_t difference = destReg.GetValue() - val;

		// I don't just (int32_t) because size matters.
		bool destSigned = destReg.GetBit((destReg.GetSize() * 8) - 1); // is the dest reg neg when signed
		bool valSigned = val & (1 << ((destReg.GetSize() * 8) - 1));   // is the src val neg when signed
		bool differenceSigned = difference & (1 << ((destReg.GetSize() * 8) - 1));   // is the difference neg when signed
		bool overflow = (!destSigned && valSigned && differenceSigned) || (destSigned && !valSigned && !differenceSigned);
		bool carry = (uint64_t)destReg.GetValue() - (uint64_t)val != (destReg.FitToSize(difference));

		Register::flags.SetBit(FlagMask::overflow, overflow);
		Register::flags.SetBit(FlagMask::carry, carry);
		Register::flags.SetBit(FlagMask::sign, differenceSigned);
		Register::flags.SetBit(FlagMask::parity, CheckParity(difference));
		Register::flags.SetBit(FlagMask::zero, difference == 0);

		mov(destReg, destReg.FitToSize(difference));
	}

	void sub(std::string dest, std::string src)
	{
		Register& reg = Register::GetReg(src);
		if(reg.GetSize() != 0)
			sub(dest, reg.GetValue());
		else
			sub(dest, Memory::GetMem<uint32_t>(Memory::labels[src]));
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

	void _and(std::string dest, uint32_t val)
	{
		Register& destReg = Register::GetReg(dest);
		mov(destReg, destReg.FitToSize(destReg.GetValue() & val));
	}

	void _and(std::string dest, std::string src)
	{
		Register& reg = Register::GetReg(src);
		if(reg.GetSize() != 0)
			_and(dest, reg.GetValue());
		else
			_and(dest, Memory::GetMem<uint32_t>(Memory::labels[src]));
	}

	void _and(OP_PARAM_TYPE args)
	{
		if(args.size() != 2)
		{
			std::cout << "\nError: and takes 2 arguments. and [reg/mem], [reg/mem/val]\n";
			return;
		}
		if(std::isdigit(args[1][0])) // is first character of the second arg a number?
			_and(args[0], std::stoi(args[1]));
		else
			_and(args[0], args[1]);
	}

	void _or(OP_PARAM_TYPE args)
	{
		if(args.size() != 2)
		{
			std::cout << "\nError: sub or 2 arguments. or [reg/mem], [reg/mem/val]\n";
			return;
		}
		if(std::isdigit(args[1][0])) // is first character of the second arg a number?
			sub(args[0], std::stoi(args[1]));
		else
			sub(args[0], args[1]);
	}

	void _or(std::string dest, uint32_t val)
	{
		Register& destReg = Register::GetReg(dest);
		mov(destReg, destReg.FitToSize(destReg.GetValue() | val));
	}

	void _or(std::string dest, std::string src)
	{
		Register& reg = Register::GetReg(src);
		if(reg.GetSize() != 0)
			_or(dest, reg.GetValue());
		else
			_or(dest, Memory::GetMem<uint32_t>(Memory::labels[src]));
	}

	void _not(std::string dest)
	{
		Register& destReg = Register::GetReg(dest);
		mov(destReg, destReg.FitToSize(~destReg.GetValue()));
	}
	
	void _not(OP_PARAM_TYPE args)
	{
		if(args.size() != 1)
		{
			std::cout << "\nError: not takes 1 arguments. not [reg/mem]\n";
			return;
		}
		_not(args[0]);
	}

	void cld(OP_PARAM_TYPE args)
	{
		Register::flags.SetBit(FlagMask::direction, 0);
	}

	void std(OP_PARAM_TYPE args)
	{
		Register::flags.SetBit(FlagMask::direction, 1);
	}

	void Init()
	{
		Register::flags.SetBit(FlagMask::parity, 1);
		Register::flags.SetBit(FlagMask::zero, 1);
		ADD_OP("add", add);
		ADD_OP("sub", sub);
		ADD_OP("cld", cld);
		ADD_OP("std", std);
		ADD_OP("and", _and);
		ADD_OP("or", _or);
		ADD_OP("not", _not);
	}
}}
