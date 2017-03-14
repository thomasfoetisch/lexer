
#include "match.hpp"

namespace regex {

bool match(const dfa& automaton, const std::string& str) {
  std::size_t current_state(automaton.start_state);
  std::string::const_iterator symbol(str.begin());
  
  bool abort(false);
  bool matched(false);
  while (not abort) {
    std::cout << "current state: " << current_state + 1 << std::endl;
    if (symbol == str.end()) {
      abort = true;
    } else {
      const std::size_t next_state(automaton.transitions[current_state][*symbol]);
      if (next_state == 0) {
        matched = false;
        abort = true;
      } else {
        current_state = next_state - 1;
        ++symbol;

        matched = (automaton.accepting_states.count(current_state) > 0);

        /*
         *  Check if the current state is marked
         */
        auto m(automaton.marked_states.find(current_state));
        if (m != automaton.marked_states.end()
            and m->second.size()) {
          std::cout << "reached mark { ";
          for (auto mm: m->second)
            std::cout << mm << " ";
          std::cout << "}" << std::endl;
        }
      }
    }
  }
  
  return matched;
}

}
