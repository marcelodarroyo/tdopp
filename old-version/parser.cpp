//=============================================================================
// File: parser.cpp
// Description: Top-Down Operator Precedence Parsing Library
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#include <iostream>
#include "parser.h"

namespace tdopp {

// parse an expression
token* parser::parse_expression(int rbp) 
{
  token* t = current_token;
  current_token = lexer.next_token();
  token* left = t->nud(this);
  while ( rbp < current_token->lbp() ) {
    t = current_token;
    current_token = lexer.next_token();
    if ( is_eoi(current_token) )
      throw parsing_error("Expected right expression");
    left = t->led(this,left);
  }
  return left;
}

bool parser::match(std::string token_name)
{
  bool result = current_token->name() == token_name;
  current_token = lexer.next_token();
  return result;
}

}
