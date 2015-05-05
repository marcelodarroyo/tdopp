//=============================================================================
// File: token.h
// Description: Top-Down Operator Precedence Parsing Library. 
//              A token contains parsing member functions (nud, led) and it 
//              also acts like an AST node.
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "error.h"

namespace tdopp {

class parser; // forward declaration

//==============================================================================
// Token class
//============================================================================== 
class token {
public:

	token(int id, int bp=-1, int lbp=-1, int arity=0)
    : _id(id), _bp(bp), _lbp(lbp), _arity(arity), _line(0), _col(0), 
      _parser(0)
    {}
    
  token(const token& other)
    : _id(other.id()), _bp(other.bp()), _lbp(other.lbp()), 
      _arity(other.arity()), _line(other.line()), _col(other.col()),
      _lexeme(other.lexeme()), _parser(other.get_parser())
    {}
    
  token operator=(const token& other) { return token(other); }

  // state
  int id() const { return _id; }
  int bp()  const { return _bp; }
  int lbp() const { return _lbp; }
  int arity() const { return _arity; }
  int line() const { return _line; }
  int col() const { return _col; }
  int lexeme() const { return _lexeme; }
  parser* get_parser() const { return _parser; }

  // state change
  void set_parser(parser* p) { _parser = p; }
  void set_lexeme(std::string l) { _lexeme = l; }

  // Some utility predicates
  bool is_eoi() const { return _id==-1; }
  bool is_literal() const { return _bp==0 && _lbp==-1 && _arity==0; }
  bool is_prefix() const { return _bp>0 && _lbp==-1; }
  bool is_infix() const { return _lbp!=-1; }
  bool is_sequence() const { return _arity==-1; }
  
  virtual ast nud() {
    throw parsing_error("Expected keyword, operand or prefix operator."); 
  }
  
  virtual ast led(ast& left) { 
    throw parsing_error("Expected infix operator."); 
  }

protected:

	int _id,             // token id
      _bp,             // binding power
      _lbp,            // left binding power
      _arity,          // operator arity
      _line,           // input position
      _col;
  std::string lexeme;  // matched string
  parser* _parser; 
};


//==============================================================================
// Some useful tokens
//============================================================================== 

// end of input token
struct end_of_input : token {
  end_of_input() : token(-1) {}
};



} // end namespace tdopp
|
#endif
