//=============================================================================
// File: parser.h
// Description: Top-Down Operator Precedence Parsing Library
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef PARSER_H
#define PARSER_H

#include "token.h"

namespace tdopp {

class tokenizer {
public:
  virtual token* next_token() { 
    return new end_of_input();
  }
};

class parser {
public:

  parser(tokenizer& t) : lexer(t)  { current_token = lexer.next_token(); }

  // Top level parsers
  token* parse_expression(int rbp=0);

  token* get_current_token() const { return current_token; }

  // auxiliary functions
  bool match(std::string token_name);
  bool reach_eoi() { return is_eoi(current_token); }

protected:

	tokenizer &lexer;
	token* current_token;
	// symbol_table scope;
};

} // end namespace tdopp

#endif
