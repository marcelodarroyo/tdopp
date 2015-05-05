simple-calculator: *.h *.cpp
	g++ -std=c++11 -g -Wfatal-errors -Werror -o $@ simple-calculator.cpp token.cpp parser.cpp

clean:
	rm simple-calculator
