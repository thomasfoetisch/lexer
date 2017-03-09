#ifndef MARKER_H
#define MARKER_H

#include <iostream>

#include <set>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class marker: public node {
 public:
  explicit marker(int id_): id(id_) {}

  virtual int initialize_positions(int start) {
    position = start;
    return start + 1;
  }

  int get_position() const { return position; }
    
  virtual bool nullable() {
    return true;
  }
  
  virtual void build_first_pos() {
    node::first_pos.insert(position);  
  }
  
  virtual void build_last_pos() {
    node::last_pos.insert(position);
  }

  virtual void populate(dfa_context& c) {
    c.markers[position] = id;
  }

  virtual node* copy() {
    return new marker(*this);
  }
  
  void print() {
    std::cout << "marker: " << id << std::endl;
    node::print();
  }

 protected:
  int position;
  int id;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* MARKER_H */
