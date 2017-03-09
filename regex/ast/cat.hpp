#ifndef CAT_H
#define CAT_H

#include <iostream>

#include <set>
#include <string>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class cat: public node {
 public:
  cat(node* l_leg, node* r_leg)
      : left_leg(l_leg), right_leg(r_leg) {
    if (not left_leg or not right_leg)
      throw std::string("both legs must be defined");
  }

  cat(const cat& op)
      : left_leg(op.left_leg->copy()), right_leg(op.right_leg->copy()) {}

  virtual int initialize_positions(int start) {
    start = left_leg->initialize_positions(start);
    start = right_leg->initialize_positions(start);
    return start;
  }
  
  virtual bool nullable() {
    return left_leg->nullable() and right_leg->nullable();
  }
  
  virtual void build_first_pos() {
    left_leg->build_first_pos();
    right_leg->build_first_pos();
    
    node::first_pos.insert(left_leg->first_pos.begin(),
                           left_leg->first_pos.end());
    if (left_leg->nullable())
      node::first_pos.insert(right_leg->first_pos.begin(),
                             right_leg->first_pos.end());
  }
  
  virtual void build_last_pos() {
    left_leg->build_last_pos();
    right_leg->build_last_pos();
    
    node::last_pos.insert(right_leg->last_pos.begin(),
                          right_leg->last_pos.end());
    if (right_leg->nullable())
      node::last_pos.insert(left_leg->last_pos.begin(),
                            left_leg->last_pos.end());
  }

  virtual void populate(dfa_context& c) {
    for (auto i: left_leg->last_pos)
      c.follow_pos[i].insert(right_leg->first_pos.begin(),
                             right_leg->first_pos.end());

    left_leg->populate(c);
    right_leg->populate(c);
  }

  virtual node* copy() {
    return new cat(*this);
  }
  
  void print() {
    std::cout << "cat" << std::endl;
    node::print();
    left_leg->print();
    right_leg->print();
  }
  
 private:
  node *left_leg, *right_leg;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* CAT_H */
