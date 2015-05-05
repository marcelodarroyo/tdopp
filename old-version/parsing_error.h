//=============================================================================
// File: parsing_error.h
// Description: Top-Down Operator Precedence Parsing Library. 
//              Parsing exception for error reporting.
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef PARSING_EXCEPTION_H
#define PARSING_EXCEPTION_H

#include "token.h"

namespace tdopp {

class parsing_error {
public:
  parsing_error(std::string what, int line=-1, int column=-1) :
  	_line(line), _col(column)
  {
    _what = "Parsing error:" + what + " at line " + std::to_string(line) +
            ", column " + std::to_string(column);
  }
    
  parsing_error(const parsing_error& other): _what(other.what()) {}
    
  std::string what() const { return _what; }

  int line() const { return _line; }
  int column() const { return _col; }
  
protected:
  std::string _what;
  int _line, _col;
};

}

#endif
