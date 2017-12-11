#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <set>

#include "../../token.hpp"
#include "../symbols.hpp"
#include "../dfa.hpp"

namespace regex {
namespace ast {


class node {
 public:
  typedef token<regex::symbol> token_type;
  
  virtual ~node() {}
  
  virtual int initialize_positions(int start) = 0;
  virtual bool nullable() = 0;
  virtual void build_first_pos() = 0;
  virtual void build_last_pos() = 0;
  virtual void populate(dfa_context& c) = 0;

  virtual node* copy() = 0;

  virtual void print() {
    std::cout << "first_pos: ";
    for (auto f: first_pos)
      std::cout << f << " ";
    std::cout << std::endl;

    std::cout << "last_pos: ";
    for (auto l: last_pos)
      std::cout << l << " ";
    std::cout << std::endl;
  }
  
  std::set<int> first_pos, last_pos;
};


}  /* namespace ast */
}  /* namespace regex */


#endif /* NODE_H */
