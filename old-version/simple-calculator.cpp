//=============================================================================
// File: simple-calculator.cpp
// Description: Top-Down Operator Precedence Parsing Library example 1
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#include <iostream>
#include <cctype>
#include "parser.h"

using namespace tdopp;

class calc_lexer : public tokenizer {
public:

  calc_lexer(std::string input) : _in(input), _pos(0) {}

  token* next_token() {
    token* result;
		if ( _pos < _in.length() ) {
      char c = _in[_pos++];
      while ( _pos < _in.length() && (c == ' ' || c == '\t') )
        c = _in[_pos++];
      if ( std::isdigit(c) || std::isalpha(c) )
        result = new literal(c);
      else
        switch(c) {
          case '+' : result = new infix("+",10);
          case '-' : result = new oper("-",9,10);
          case '*' : result = new infix("*",20);
          case '/' : result = new infix_r("/",20);
          case '(' : result = new opener("(",")");
          case ')' : result = new closer(")");
          case '=' : result = new infix_r("=",5);
          case ',' :
          case ';' :
          case '\n': result = new infix("seq",1);
          default  : throw parsing_error("Unknown token.");
        }
    } 
    else
      result = new end_of_input();
    std::cout << "log: current_token=" << result->name() << std::endl;
    return result;
  }

private:
	std::string _in;
	int _pos;
};

int main(int argc, char* argv[])
{
  bool in_cmd_line = argc > 1;
  int args_processed = 0;

  while ( true ) {
    std::string input;

    if ( in_cmd_line )
      if ( args_processed < argc-1 )
        input = argv[++args_processed];
      else
        break;
    else {
      std::cout << "> ";
      std::cin >> input;
      if ( std::cin.eof() ) break;
    }

    calc_lexer lexer(input);
    parser p(lexer);

    try {
      token* ast = p.parse_expression();
      std::cout << ast->to_string() << std::endl;
    }
    catch(parsing_error& error) {
      std::cerr << error.what() << std::endl;
      return 2;
    }
    if ( p.reach_eoi() )
      std::cout << "Parsing OK." << std::endl;
    else {
      std::cout << "Parsing error at token " << p.get_current_token()->name();
      if ( is_literal( p.get_current_token() ) )
        std::cout << " with value " + static_cast<literal*>(p.get_current_token())->value();
      std::cout << std::endl;
    }
  }

  return 0;
}
