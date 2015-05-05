//=============================================================================
// File: ast.h
// Description: Abstract Syntax Tree
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef AST_H
#define AST_H

#include <string>
#include <vector>

namespace tdopp {

struct ast {

  ast(int id) 
    : node_id(id), parent(nullptr), _line(0), _col(0), _length(0)
    {}

  ~ast() { for(auto c : childs) if (c) delete c; }

  void add_child(ast* child) { 
    childs.push_back(child);
    child->set_parent(this);
  }

  void set_input(int line, int col, int length) { 
    _line = line; _col = col; _length = length;
  }

  int id() const { return node_id; }
  ast* child(int i) { return childs[i]; }
  int childs_no() const { return childs.size(); }
  int line() const { return _line; }
  int col() const { return col; }
  int length() const { return _length; }

protected:

  int node_id;
  std::vector< ast* > childs;
  ast* parent;
  int _line, _col, _length; // input data
};

ast* create_ast_node(int id) {  }

} // end namespace tdopp

#endif