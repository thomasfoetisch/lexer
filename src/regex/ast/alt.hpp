#ifndef ALT_H
#define ALT_H

#include <iostream>

#include <set>
#include <string>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class alt: public node {
 public:
  using node::token_type;

  alt(node* l_leg, node* r_leg, token_type* t)
      : left_leg(l_leg), right_leg(r_leg), coordinates(t->render()) {}

  alt(node* l_leg, node* r_leg)
      : left_leg(l_leg), right_leg(r_leg), coordinates() {}

  alt(const alt& op)
      : left_leg(op.left_leg->copy()),
        right_leg(op.right_leg->copy()),
        coordinates(op.coordinates) {}
  
  virtual int initialize_positions(int start) {
    start = left_leg->initialize_positions(start);
    start = right_leg->initialize_positions(start);
    return start;
  }

  virtual bool nullable() {
    return left_leg->nullable() or right_leg->nullable();
  }
  
  virtual void build_first_pos() {
    left_leg->build_first_pos();
    right_leg->build_first_pos();
    
    node::first_pos.insert(left_leg->first_pos.begin(),
                           left_leg->first_pos.end());
    node::first_pos.insert(right_leg->first_pos.begin(),
                           right_leg->first_pos.end());
  }
  
  virtual void build_last_pos() {
    left_leg->build_last_pos();
    right_leg->build_last_pos();
    
    node::last_pos.insert(left_leg->last_pos.begin(),
                          left_leg->last_pos.end());
    node::last_pos.insert(right_leg->last_pos.begin(),
                          right_leg->last_pos.end());
  }

  virtual void populate(dfa_context& c) {
    left_leg->populate(c);
    right_leg->populate(c);
  }

  virtual node* copy() {
    return new alt(*this);
  }
  
  void print() {
    std::cout << "alt: " << coordinates << std::endl;
    node::print();
    left_leg->print();
    right_leg->print();
  }

 private:
  node *left_leg, *right_leg;

  const std::string coordinates;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* ALT_H */
