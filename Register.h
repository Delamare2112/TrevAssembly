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

	size_t size;
	bool internallyAllocated;
	uint32_t* value;

public:
	static Register flags;

	Register(size_t size = 4, uint32_t* addr = nullptr);

	~Register();

	static void Init();

	uint32_t* GetAddr();

	void SetValue(uint32_t val);

	uint32_t GetValue() const;

	bool GetBit(uint8_t bit);

	void SetBit(uint8_t index, bool value);

	uint32_t GetMaxValue();

	size_t GetSize();

	uint32_t FitToSize(uint32_t value);

	static Register& GetReg(std::string regname);

	friend std::ostream& operator<<(std::ostream& os, const Register& reg);
};
