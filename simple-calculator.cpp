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

enum token_types {
  // literals
  num_t,
  var_t,
  // arithmetic operators
  plus_t,
  minus_t,
  mult_t,
  divide_t,
  // parenthesis (opener, closer)
  opar_t,
  cpar_t,
  // assignment
  assign_t,
  // conditional (ternary) expression
  cond_t,
  cond2_t,
  // sequence operator
  seq_t
}; 

class calc_lexer : public tokenizer {
public:

  calc_lexer(std::string input) : _in(input), _pos(0) {}

  token* next_token() {
    token* result;
		if ( _pos < _in.length() ) {
      char c = _in[_pos++];
      // skip blanks
      while ( _pos < _in.length() && (c == ' ' || c == '\t') )
        c = _in[_pos++];
      if ( std::isdigit(c) )
        result = new literal(num_t, std::string(&c,1));
      else if ( std::isalpha(c) )
        result = new literal(var_t, std::string(&c,1));
      else
        switch(c) {
          case '+' : result = new infix(plus_t,10);               break;
          case '-' : result = new oper(minus_t,9,10,2);           break;
          case '*' : result = new infix(mult_t,20);               break;
          case '/' : result = new infix_r(divide_t,20);           break;
          case '(' : result = new opener(opar_t,cpar_t);          break;
          case ')' : result = new closer(cpar_t);                 break;
          case '=' : result = new infix_r(assign_t,5);            break;
          case '?' : result = new midfix(cond_t,30,3,-1,cond2_t); break;
          case ':' : result = new midfix(cond2_t,30,2,cond_t,-1); break;
          case ',' :
          case ';' :
          case '\n': result = new infix(seq_t,1);   break;
          default  : throw parsing_error("Unknown token.");
        }
    } 
    else
      result = new end_of_input();
    alloc_tokens++;
    return result;
  }

  std::string token_name(int token_id) {
    switch(token_id) {
      case num_t:     return "num";
      case var_t:     return "var";
      case plus_t:    return "+";
      case minus_t:   return "-";
      case mult_t:    return "*";
      case divide_t:  return "/";
      case opar_t:    return "(";
      case cpar_t:    return ")";
      case assign_t:  return "=";
      case cond_t:    return "?:";
      case cond2_t:   return ":";
      case seq_t:     return ",";
      default: return "Unknown";
    }
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
    token_ptr ast;

    try {
      ast = p.parse_expression();
    }
    catch(parsing_error& error) {
      std::cerr << error.what() << std::endl;
      return 2;
    }
    if ( p.reach_eoi() )
      std::cout << "Parsing OK." << std::endl;
    else
      std::cout << "Parsing error at token " 
                << p.token_name(p.current_token()->id()) 
                << std::endl;
    release(ast);
  }
  std::cout << "Allocated tokens:" << alloc_tokens 
            << " Freed tokens:" << released_tokens << std::endl;
  return 0;
}
