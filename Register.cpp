#include "Register.h"

std::unordered_map<std::string, Register> Register::registers;
Register Register::dummyReg = Register(0);
Register Register::flags = Register(2);

Register::Register(size_t size, uint32_t* addr)
{
	this->size = size;
	if(addr == nullptr)
	{
		internallyAllocated = true;
		addr = new uint32_t;
	}
	else
	{
		internallyAllocated = false;
	}
	value = addr;
}

Register::~Register()
{
	if(internallyAllocated)
		delete value;
}

void Register::Init()
{
	registers["eax"] = Register(4);
	registers["ax"] = Register(2, registers["eax"].value);
	registers["ah"] = Register(1, (uint32_t*)((char*)registers["eax"].value + 1));
	registers["al"] = Register(1, registers["eax"].value);

	registers["ebx"] = Register(4);
	registers["bx"] = Register(2, registers["ebx"].value);
	registers["bh"] = Register(1, (uint32_t*)((char*)registers["ebx"].value + 1));
	registers["bl"] = Register(1, registers["ebx"].value);

	registers["ecx"] = Register(4);
	registers["cx"] = Register(2, registers["ecx"].value);
	registers["ch"] = Register(1, (uint32_t*)((char*)registers["ecx"].value + 1));
	registers["cl"] = Register(1, registers["ecx"].value);

	registers["edx"] = Register(4);
	registers["dx"] = Register(2, registers["edx"].value);
	registers["dh"] = Register(1, (uint32_t*)((char*)registers["edx"].value + 1));
	registers["dl"] = Register(1, registers["edx"].value);
}

uint32_t* Register::GetAddr() 
{
	return value;
}

void Register::SetValue(uint32_t val)
{
	switch(size)
	{
	case 1:
		*((uint8_t*)value) = (uint8_t)val; break;
	case 2:
		*((uint16_t*)value) = (uint16_t)val; break;
	case 4:
		*((uint32_t*)value) = (uint32_t)val; break;
	}
}

uint32_t Register::GetValue() const
{
	switch(size)
	{
	case 1:
		return (uint8_t)*value;
	case 2:
		return (uint16_t)*value;
	case 4:
		return (uint32_t)*value;
	}
	return 0;
}

bool Register::GetBit(uint8_t bit)
{
	return *value & (1 << bit);
}

void Register::SetBit(uint8_t index, bool value)
{
	if(value)
		SetValue(GetValue() | (1 << index));
	else
		SetValue(GetValue() & ~(1 << index));
}

uint32_t Register::GetMaxValue()
{
	return (uint32_t)(std::pow(2, GetSize() * 8) - 1);
}

size_t Register::GetSize() 
{
	return size;
}

uint32_t Register::FitToSize(uint32_t value)
{
	return value & GetMaxValue();
}

Register& Register::GetReg(std::string regname)
{
	auto regIter = registers.find(regname);
	if(regIter != registers.end())
		return regIter->second;
	std::cout << "Register with name '" << regname + "' could not be found\n";
	return Register::dummyReg;
}

std::ostream& operator<<(std::ostream& os, const Register& reg)
{
    os << reg.GetValue();
    return os;
}
