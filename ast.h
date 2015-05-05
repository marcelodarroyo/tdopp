//=============================================================================
// File: ast.h
// Description: Abstract Syntax Tree
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef AST_H
#define AST_H

#include "tree.h"

namespace tdopp {

struct ast_info {
  ast_info(int identifier, int begin_pos, int end_pos)
  : id(identifier), begin(begin_pos), end(end_pos)
  {}

  int id,
      begin,  // input position
      end;
};

using ast = std::unique_ptr< tree<ast_info> >;

ast ast_node(int id, int begin=0, int end=0)
{
  return tree_node(ast_info(id,begin,end));
}

} // end namespace tdopp

#endif