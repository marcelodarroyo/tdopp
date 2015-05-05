//=============================================================================
// File: parser.h
// Description: Top-Down Operator Precedence Parsing Library
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include "error.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"

namespace tdopp {

struct parser {

  parser(tokenizer& lex) : lexer(lex), current(nullptr) {}

  template<typename Parser>
  ast parse(std::istream& input) { 
    lexer.set_input(input);
    Parser parser(lexer);
    next_token();
    return parser.parse();
  }

  void register_token(std::string reg_exp, token tok) {
    lexer.register_token(reg_exp,tok);
  }

  ast parse_expression(int rbp=0) {
    token* t = current;
    next_token();
    ast tree = t->nud();
    while ( rbp < current->lbp() ) {
      t = current;
      next_token();
      tree = t->led(tree);
    }
    return tree;
  }
  
  //==============================================================================
  // some usefull parser combinators (for using in token nud() and led())
  //==============================================================================
  // sequence
  template<typename Parser, typename... Others>
  ast seq(const Parser& first, const Others& ... others) {
    ast tree = first.parse();
    if (sizeof...(others) > 0)
      tree.add_child(seq(others...));
    return tree;
  }

  // priorized choice 
  template<typename Parser, typename... Others>
  ast alt(const Parser& first, const Others& ... others) {
    try {
      int pos = lexer.position();
      return first.parse();
    } catch (parsing_error& e) { 
      if (sizeof...(others) > 0) {
        lexer.position(pos);    // backtrack
        return alt(others...);
      }
    }
  }

  // optional (zero or one)
  template<typename Parser>
  ast opt(const Parser& parser) {
    try {
      int pos = lexer.position();
      return = parser.parse();
    }
    catch (parsing_error& e) {
      lexer.position(pos);    // backtrack
      return ast();           // return empty ast
    }
  }

  // zero or more
  template<typename Parser, typename Separator=parselet>
  ast many(const Parser& parser, const Separator& sep) {
    ast tree;
    int pos = lexer.position();
    while (true) { 
      try {
        tree.add_child(parser.parse());
        sep.parse();
      } 
      catch (parsing_error& e) {
        if ( e.failed_parser() == parser )
          lexer.position(pos); // backtrack
        break;
      }
    }
    return tree;
  }

  // one or more
  template<typename Parser, typename Separator=parselet>
  ast many1(const Parser& parser, const Separator& sep) {
    ast tree = many(parser,sep);
    if ( !tree.has_childs() )
      throw parsing_error(parser);
    return tree;
  }
  
protected:

  void next_token() { 
    current = lexer.next_token();
    current->set_parser(this); 
  }
  
  token* current;
  tokenizer& lexer;
}; // end parser class
  
} // end namespace tdopp

#endif
