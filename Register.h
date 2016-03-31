#pragma once
#include <unordered_map>
#include <iostream>
#include <string>

enum FlagMask
{
	carry = 0,
	parity = 2,
	adjust = 4,
	zero = 6,
	sign = 7,
	trap = 8,
	interuptEnable = 9,
	direction = 10,
	overflow = 11,
};

class Register
{
private:
	static std::unordered_map<std::string, Register> registers;
	static Register dummyReg;
	static Register flags;

	size_t size;
	bool internallyAllocated;
	uint32_t* value;

public:

	Register(size_t size = 4, uint32_t* addr = nullptr);

	~Register();

	static void Init();

	uint32_t* GetAddr();

	void SetVal(uint32_t val);

	uint32_t GetValue() const;

	bool GetBit(uint8_t bit);

	size_t GetSize();

	static Register& GetReg(std::string regname);

	friend std::ostream& operator<<(std::ostream& os, const Register& reg);
};
