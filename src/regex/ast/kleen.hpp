#ifndef KLEEN_H
#define KLEEN_H

#include <iostream>
#include <string>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class kleen: public node {
 public:
  using node::token_type;
  
  kleen(node* l, token_type* t): leg(l), coordinates(t->render_full()) {}

  kleen(const kleen& op)
      : leg(op.leg->copy()), coordinates(op.coordinates) {}

  virtual int initialize_positions(int start) {
    return leg->initialize_positions(start);
  }
  
  virtual bool nullable() {
    return true;
  }
  
  virtual void build_first_pos() {
    leg->build_first_pos();
    node::first_pos = leg->first_pos;
  }
  
  virtual void build_last_pos() {
    leg->build_last_pos();
    node::last_pos = leg->last_pos;
  }

  virtual void populate(dfa_context& c) {
    for (auto i: leg->last_pos)
      c.follow_pos[i].insert(leg->first_pos.begin(),
                             leg->first_pos.end());

    leg->populate(c);
  }

  virtual node* copy() {
    return new kleen(*this);
  }

  void print() {
    std::cout << "kleen: " << coordinates << std::endl;
    node::print();
    leg->print();
  }

 private:
  node *leg;

  const std::string coordinates;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* KLEEN_H */
