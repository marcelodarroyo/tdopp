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
#include <vector>
#include "parsing_error.h"

namespace tdopp {

class parser; // see parser.h

// tokens base class
class token {
public:

	token(std::string name, int bp=-1, int lbp=-1) : 
    _name(name), _bp(bp), _lbp(lbp), _line(-1), _col(-1)
  {}

  virtual ~token() {}

	// null denotation: general parser for prefix operators, constants, ... 
  virtual token* nud(parser* p) { 
    throw parsing_error("Unknown literal or prefix", _line, _col);
  }

	// left denotation: parser for infix and postfix operators
	virtual token* led(parser* p, token* left) {
	  throw parsing_error("Unknown operator", _line, _col);
	}

  // state
  std::string name() { return _name; }
  int bp()  const { return _bp; }
  int lbp() const { return _lbp; }

	// set/get source code position
	void src_position(int line, int col) { _line=line; _col=col; }
	int  src_line() const { return _line; }
	int  src_column() const { return _col; }
  void src_length(int length) { _len = length; }
  int  src_length() const { return _len; }

  // serialization
  virtual std::string to_string() { return _name; }

protected:

	int _line,
	    _col,
      _len,
      _bp,
      _lbp;

  std::string _name;
};

//==============================================================================
// Some useful token types
//==============================================================================

// end of input
class end_of_input : public token {
public:
  end_of_input() : token("_EOI_") {}

  token* nud(parser* p) { 
    throw parsing_error("Unexpected end of input", _line, _col);
  }

  token* led(parser* p, token* left) {
    throw parsing_error("Unexpected end of input", _line, _col);
  }
};

// a literal (constant)
class literal : public token {
public:
  literal(std::string value) : token("LIT", 0), _value(value) {}

  literal(char value) : token("LIT", 0), _value(&value,1) {}

  std::string value() const { return _value; }

  token* nud(parser* p) { return this; }

  std::string to_string() { return _value; }

protected:
  std::string _value;
};

// operator (an AST internal node)
class oper : public token {
public:
  oper(std::string name, int bp, int lbp) : token(name, bp, lbp) {}
 
  virtual ~oper() { for (auto c : _childs) delete c; }

  token* nud(parser* p);
  token* led(parser* p, token* left);
 
  void add_child(token* t) { _childs.push_back(t); }
  token* child(int i) { return _childs[i]; }

  std::string to_string();

protected:
  std::vector<token*> _childs;
};

// prefix (unary) operators
class prefix : public oper {
public:
  prefix(std::string name, int bp) : oper(name, bp, -1) {}
};

// infix (binary) operators
class infix : public oper {
public:
  infix(std::string name, int lbp) : oper(name, -1, lbp) {}
};

// infix (binary) right associative operators
class infix_r : public oper {
public:
  infix_r(std::string name, int lbp) : oper(name, -1, lbp) {}

  token* led(parser* p, token* left);
};

// opener: (, {, ...
class opener : public prefix {
public:
  opener(std::string name, std::string closer_name) : 
    prefix(name, 0), _closer_name(closer_name) 
  {}

  token* nud(parser* p);
protected:
  std::string _closer_name;
};

// closer: ), }, ...
class closer : public token {
public:
  closer(std::string name) : token(name, 0, 0) {}
};

// sequences
class sequence : public infix {
  sequence(std::string name, int precedence) :
    infix(name,precedence)
  {}

  token* led(parser* p, token* left);
};

//=============================================================================
// Some utility predicates
//=============================================================================
bool is_eoi(token* t);
bool is_literal(token* t);

} // end namespace tdopp

#endif
