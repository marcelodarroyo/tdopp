test_ast: test_ast.cpp ast.h tree.h
	g++ -std=c++11 -Wfatal-errors -Werror -o $@ test_ast.cpp

simple-calculator: *.h *.cpp
	g++ -std=c++11 -g -Wfatal-errors -Werror -o $@ simple-calculator.cpp token.cpp parser.cpp

clean:
	rm simple-calculator
