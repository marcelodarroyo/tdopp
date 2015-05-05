#include <iostream>
#include "ast.h"

using namespace tdopp;

int main()
{
  ast root = ast_node(1);
  ast child1 = ast_node(2);

  root->add_child(child1);

  std::cout << "root: " << root->data.id << " child1: " << root->child(0).data.id << std::endl;

  return 0;
}