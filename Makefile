all:
	@echo "Compiling..."
	@g++ -Werror=return-type -g -std=c++11 *.cpp -o program
	@echo "done"
