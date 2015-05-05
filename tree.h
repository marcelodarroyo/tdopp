//=============================================================================
// File: tree.h
// Description: Generic tree
// Author: Marcelo Arroyo (2015)
// Licence: LGPL v3 (see licence.txt)
//=============================================================================

#ifndef TREE_H
#define TREE_H

#include <memory>
#include <vector>

namespace tdopp {

template<typename T>
struct tree {

  tree(T info) : data(info), parent(nullptr) {}

  tree(tree&& other) : 
    data(std::move(other.data)), parent(std::move(other.parent)), childs(std::move(other.childs))
  {}

  ~tree() { std::cout << "deleting tree\n"; }

  tree& operator=(tree&& other)  {
    data = std::move(other.data);
    parent = std::move(other.parent);
    childs = std::move(other.childs); 
  }

  void add_child(std::unique_ptr< tree<T> >& child) {
    childs.push_back(std::move(child));
  }

  tree<T>& child(int i) { return *(childs[i]); }
  int childs_no() const { return childs.size(); }

  T data;

protected:

  std::unique_ptr< tree<T> > parent;
  std::vector< std::unique_ptr< tree<T> > > childs;
};

// ast tree node creation
template<typename T>
std::unique_ptr< tree<T> > tree_node(T data)
{
  return std::unique_ptr< tree<T> >(new tree<T>(data));
}

} // end namespace tdopp

#endif