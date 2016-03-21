#include "Register.h"

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

void mov(std::string regname, uint32_t val)
{
	Register& reg = Register::GetReg(regname);
	if(reg.GetSize() < GetBytesNeeded(val))
		throw 	"lhs(" + regname + ") and rhs(" + std::to_string(val) + ") must be of compatible size.\n"
				"lhs and rhs are of size " + std::to_string(reg.GetSize()) + " and " + std::to_string(GetBytesNeeded(val)) + " bytes respecively.";
	reg.SetVal(val);
}

int main()
{
	Register::Init();
	try
	{
		mov("eax", 4294967295);
		mov("ax", 300);
		mov("al", 1);
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
