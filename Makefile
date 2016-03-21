all:
	@echo "Compiling..."
	@clang++ -g -std=c++14 *.cpp -o program
	@echo "done"