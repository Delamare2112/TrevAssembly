#include "Register.h"
#include "Commands.h"

int main(int argc, char* argv[])
{
	Commands::Init();
	Register::Init();
	Commands::Command userInput;
	std::cout << "Welcome to TrevAssembly! Plz enter asm commands!\n";
	do
	{
		userInput = Commands::GetCommand();
		userInput.Execute();

	}while (userInput.op != "exit");
	return 0;
}
