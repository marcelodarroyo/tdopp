//==============================================================================
// File: token.cpp
// Description: Top-Down Operator Precedence Parsing Library. token classes
//              implementation.
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//==============================================================================

#include <iostream>
#include "token.h"
#include "parser.h"

namespace tdopp {

void error(token& t, std::string msg, int line, int column)
{
  throw parsing_error(msg, line, column);
}

//============================ oper implementation ============================
token_ptr oper::nud(parser& p)
{
  for (int i; i<_arity; i++)
    add_child(p.parse_expression(_bp));
  return this;
}

token_ptr oper::led(parser& p, token_ptr left)
{
  add_child(left);
  add_child(p.parse_expression(_lbp));
  return this;
}

//========================== infix_r implementation ============================
token_ptr infix_r::led(parser& p, token_ptr left)
{
  add_child(left);
  add_child(p.parse_expression(_lbp-1));
  return this;
}

//========================== midfix implementation ============================
token_ptr midfix::nud(parser& p)
{
  token_ptr right = p.parse_expression(_bp);
  if ( right->id() != _next )
    error(right, "Expected " + p.token_name(_next), _line, _col);
  add_child(right);
  return this;
}

token_ptr midfix::led(parser& p, token_ptr left)
{
  if ( left->id() != _previous )
    error(this, "Unexpected token", _line, _col);
  token_ptr right = p.parse_expression(_lbp);
  if ( right->id() != _next )
    error(right, "Expected " + p.token_name(_next), _line, _col);
  to_oper(left)->add_child( to_oper(right)->take_child() );
  return left;
}

//========================= opener implementation ==============================
token_ptr opener::nud(parser& p)
{
  token_ptr right = p.parse_expression(_bp), current = p.current_token();

  if ( current->id() != _closer ) {
    std::string msg = "Expected " + p.token_name(_closer) + 
                      " got " + p.token_name(current->id());
    error(right, msg, _line, _col);
  }
  p.advance();
  return right;
}

//=========================== sequence (separators) ============================
token_ptr sequence::led(parser& p, token_ptr left)
{
  token_ptr result = left->id() == _id ? left : this;
  static_cast<sequence*>(result)->add_child( p.parse_expression(_lbp) );
  return result;
}

} // end namespace tdopp
