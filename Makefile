all:
	@echo "Compiling..."
	@clang++ -Werror=return-type -g -std=c++1z *.cpp -o program
	@echo "done"
