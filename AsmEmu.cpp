#include "Register.h"
#include "ALU.h"

using Commands::Command;

int main(int argc, char* argv[])
{
	Commands::Init();
	Commands::ALU::Init();
	Register::Init();
	std::cout << "Welcome to TrevAssembly! Plz enter asm commands!\n";
	do
	{
		Command::currentCommand = Commands::GetCommand();
		Command::currentCommand.Execute();

	}while (Command::currentCommand.op != "exit");
	return 0;
}
