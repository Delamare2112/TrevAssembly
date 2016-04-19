#include "Memory.h"

namespace Commands { namespace Memory
{
	std::vector<char*> memorySegments;
	std::unordered_map<std::string, uint> labels;
	std::unordered_map<std::string, uint> constants;

	uint PC = 0; // Let's just say our application is now at this place for testing purposes

	void IncrementPC(size_t size = 1)
	{
		PC += size;
		if(PC > MemSegmentSize * memorySegments.size())
			memorySegments.push_back(new char[MemSegmentSize]);
	}

	template <typename T>
	void PutMem(std::vector<std::string> values) {
		for(std::string v : values) {
			*(T*)(memorySegments[PC / MemSegmentSize] + PC % MemSegmentSize) = (T)std::stoi(v);
			IncrementPC(sizeof(T));
		}
	}

	void BYTE(OP_PARAM_TYPE args)
	{
		labels[args[0]] = PC;
		args.erase(args.begin());
		PutMem<uint8_t>(args);
	}
	void WORD(OP_PARAM_TYPE args)
	{
		labels[args[0]] = PC;
		args.erase(args.begin());
		PutMem<uint16_t>(args);
	}
	void DWORD(OP_PARAM_TYPE args)
	{
		labels[args[0]] = PC;
		args.erase(args.begin());
		PutMem<uint32_t>(args);
	}
	void Init()
	{
		memorySegments.push_back(new char[MemSegmentSize]);
		ADD_OP("BYTE", BYTE);
		ADD_OP("WORD", WORD);
		ADD_OP("DWORD", DWORD);
	}
}}
