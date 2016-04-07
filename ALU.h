#pragma once
#include "Commands.h"

// namespace Commands::ALU
namespace Commands { namespace ALU
{
	bool CheckParity(uint8_t bits);
	void add(std::string dest, uint32_t val);
	void add(std::string dest, std::string src);
	void add(OP_PARAM_TYPE args);
	void sub(std::string dest, uint32_t val);
	void sub(std::string dest, std::string src);
	void sub(OP_PARAM_TYPE args);
	void cld(OP_PARAM_TYPE args);
	void std(OP_PARAM_TYPE args);
	void Init();
}}
