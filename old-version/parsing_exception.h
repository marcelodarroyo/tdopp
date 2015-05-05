//=============================================================================
// File: parsing_error.h
// Description: Top-Down Operator Precedence Parsing Library. 
//              Parsing exception for error reporting.
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef PARSING_EXCEPTION_H
#define PARSING_EXCEPTION_H

#include <stdexept>

class parsing_error : public runtime_error {
public:
  parsing_error(const char * what_arg) : runtime_error(what_arg) {}
  
};

#endif
