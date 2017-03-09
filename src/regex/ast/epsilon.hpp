#ifndef EPSILON_H
#define EPSILON_H

#include <iostream>
#include <set>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class epsilon: public node {
 public:
  epsilon() {}

  virtual int initialize_positions(int start) {
    return start;
  }
  
  virtual bool nullable() {
    return true;
  }
  
  virtual void build_first_pos() {}
  virtual void build_last_pos() {}

  virtual void populate(dfa_context&) {}

  virtual node* copy() {
    return new epsilon(*this);
  }
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* EPSILON_H */
