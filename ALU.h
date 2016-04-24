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
	void _and(std::string dest, uint32_t src);
	void _and(std::string dest, std::string src);
	void _and(OP_PARAM_TYPE args);
	void _not(std::string dest);
	void _not(OP_PARAM_TYPE args);
	void _or(std::string dest, uint32_t src);
	void _or(std::string dest, std::string src);
	void _or(OP_PARAM_TYPE args);
	void cld(OP_PARAM_TYPE args);
	void std(OP_PARAM_TYPE args);
	void Init();
}}
