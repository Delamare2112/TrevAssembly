all:
	@echo "Compiling..."
	@clang++ -Werror=return-type -g -std=c++14 *.cpp -o program
	@echo "done"
