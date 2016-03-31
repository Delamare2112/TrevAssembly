#include "ALU.h"
#include <cmath>

namespace Commands { namespace ALU
{
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

		mov(destReg, destReg.FitToSize(sum));
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

		mov(destReg, destReg.FitToSize(difference));
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
