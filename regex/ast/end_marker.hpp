#ifndef END_MARKER_H
#define END_MARKER_H

#include <iostream>

#include <set>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class end_marker: public node {
 public:
  using node::token_type;
  
  virtual int initialize_positions(int start) {
    position = start;
    return start + 1;
  }

  virtual bool nullable() { return false; }

  virtual void build_first_pos() {
    node::first_pos.insert(position);
  }
  
  virtual void build_last_pos() {
    node::last_pos.insert(position);
  }

  virtual void populate(dfa_context& c) {}

  virtual node* copy() {
    return new end_marker(*this);
  }

 private:
  int position;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* END_MARKER_H */
