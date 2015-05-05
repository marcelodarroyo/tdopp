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

//========================= oper implementation ================================
token* oper::nud(parser* p)
{
  add_child(p->parse_expression(_bp));
  return this;
}

token* oper::led(parser* p, token* left)
{
  add_child(left);
  add_child(p->parse_expression(_lbp));
  return this;
}

std::string oper::to_string()
{
  std::string s = '(' + _name;
  for(token* t : _childs)
    s += ' ' + t->to_string();
  return s + ')'; 
}

//========================== infix_r implementation ============================
token* infix_r::led(parser* p, token* left)
{
  add_child(left);
  add_child(p->parse_expression(_lbp-1));
  return this;
}

//========================= opener implementation ==============================
token* opener::nud(parser* p)
{
  token* t = p->parse_expression(_bp);
  if ( !p->match(_closer_name) )
    throw parsing_error("Expected " + _closer_name, src_line(), src_column());
  return t;
}

//=========================== sequence (separators) ============================
token* sequence::led(parser* p, token* left)
{
  token* result = left->name() == name() ? left : this;
  static_cast<sequence*>(result)->add_child( p->parse_expression(_lbp) );
  return result;
}

//=============================================================================
// Some utility predicates
//=============================================================================
bool is_eoi(token* t)
{
  return t->name() == "_EOI_";
}

bool is_literal(token* t)
{
  return t->name() == "LIT";
}

} // end namespace tdopp
