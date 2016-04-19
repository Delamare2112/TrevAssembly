#pragma once
#include "Commands.h"

namespace Commands { namespace Memory
{
	const uint MemSegmentSize = 10485760; // 10MiB

	extern std::vector<char*> memorySegments;
	extern std::unordered_map<std::string, uint> labels;
	extern std::unordered_map<std::string, uint> constants;
	void BYTE(OP_PARAM_TYPE args);
	void WORD(OP_PARAM_TYPE args);
	void DWORD(OP_PARAM_TYPE args);
	void Init();

	template<typename T>
	T GetMem(uint addr)
	{
		return *(memorySegments[addr / MemSegmentSize] + addr % MemSegmentSize);
	}
}}
