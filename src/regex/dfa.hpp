#ifndef DFA_H
#define DFA_H

#include <iostream>

#include <vector>
#include <map>
#include <set>

namespace regex {

namespace ast {
class char_class;
}


struct dfa_context {
  std::map<int, ast::char_class*> positions;
  std::map<int, std::set<int> > follow_pos;
  std::map<int, int> markers;
  
  
  void print() {
    for (auto m: markers)
      std::cout << "marker " << m.second << " at " << m.first << std::endl;

    for (auto p: positions)
      std::cout << p.first << " ";
    std::cout << std::endl;
    
    for (auto p: follow_pos) {
      std::cout << p.first << " = { ";
      for (auto i: p.second)
        std::cout << i << " ";
      std::cout << "}" << std::endl;
    }
  }
};

struct dfa {
  std::vector<std::vector<std::size_t> > transitions;
  std::ptrdiff_t start_state;
  std::set<std::size_t> accepting_states;
  std::map<int, std::set<int> > marked_states;

  void clear() {
    transitions.clear();
    start_state = 0;
    accepting_states.clear();
    marked_states.clear();
  }
};


}  /* namespace regex */

#endif /* DFA_H */
