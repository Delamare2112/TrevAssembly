#include <unordered_map>
#include <iostream>
#include <string>

class Register
{
private:
	static std::unordered_map<std::string, Register> registers;
	static Register dummyReg;

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

	size_t GetSize();

	static Register& GetReg(std::string regname);

	friend std::ostream& operator<<(std::ostream& os, const Register& reg);
};
