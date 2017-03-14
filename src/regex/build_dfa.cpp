
#include <iostream>
#include <iterator>
#include <map>
#include <stack>
#include <set>

#include "build_dfa.hpp"


namespace regex {

dfa build_dfa(dfa_context& c, ast::node* root) {
  typedef std::set<int> state;
  std::set<state> dfa_state;
  std::stack<state> stack;

  std::map<state, std::map<unsigned char, state> > transitions;

  const int end_marker_state(root->initialize_positions(0));
  const state start_state(root->first_pos);
  stack.push(start_state);
  dfa_state.insert(start_state);
  
  while (stack.size()) {
    std::set<int> current(stack.top());
    stack.pop();

    for (unsigned int sym(0); sym < 256; ++sym) {
      /*
       *  Construct the new state
       */
      state U;
      for (auto p: current)
        if (p != end_marker_state
            and c.positions[p]
            and c.positions[p]->match(sym))
          U.insert(c.follow_pos[p].begin(),
                   c.follow_pos[p].end());

      /*
       *  Proceed only if the set is non-empty
       */
      if (U.size()) {
        
        /*
         *  Decide if the new state is marked or not
         */
        if (dfa_state.count(U) == 0) {
          stack.push(U);
          dfa_state.insert(U);
        }

        /*
         *  Record the new transition
         */
        transitions[current][sym] = U;
      }
    }
  }


  /*
   *  Convert the transition function from map<, map<,> > to vector<vector<>>
   */
  std::vector<std::vector<std::size_t> >
      indexed_transitions(dfa_state.size(), std::vector<std::size_t>(256, 0));
  for (auto symbol_map: transitions)
    for (auto symbol: symbol_map.second)
      indexed_transitions
          [std::distance(dfa_state.begin(),
                         dfa_state.find(symbol_map.first))]
          [symbol.first]
          = std::distance(dfa_state.begin(),
                          dfa_state.find(symbol.second)) + 1;

  
  /*
   *  Identify the accepting states
   */
  std::set<std::size_t> accepting_states;
  for (auto s: dfa_state)
    if (s.count(end_marker_state) > 0)
      accepting_states.insert(std::distance(dfa_state.begin(),
                                            dfa_state.find(s)));


  /*
   *  Identify the marked states
   */
  std::map<int, std::set<int>> marked_states;
  for (auto s: dfa_state)
    for (auto m: c.markers)
      if (s.count(m.first)) {
        const std::size_t state_id(std::distance(dfa_state.begin(),
                                                 dfa_state.find(s)));
        //marked_states[state_id].insert(m.second);
        marked_states[m.second].insert(state_id);
      }

  
  /*
   *  Identify the start state
   */
  const std::ptrdiff_t start_state_id(std::distance(dfa_state.begin(),
                                                    dfa_state.find(start_state)));

  return dfa {
      indexed_transitions,
      start_state_id,
      accepting_states,
      marked_states
    };


  
  /*
   *  Print the transition table
   */
  for (int t(0); t < indexed_transitions.size(); ++t) {
    std::cout << t + 1 << ": ";
    //for (int s('a'); s < 'z'; ++s)
    for (int s(32); s < 'z'; ++s)
      std::cout << indexed_transitions[t][s] << " ";
    std::cout << std::endl;
  }


  /*
   *  Print the set of accepting states
   */
  std::cout << "accepting states: ";
  for (auto s: accepting_states)
    std::cout << s + 1 << " ";
  std::cout << std::endl;

  
  /*
   *  Print the dfa states
   */
  std::cout << dfa_state.size() << " states:" << std::endl;
  for (auto s: dfa_state) {
    std::cout << "{ ";
    for (auto i: s)
      std::cout << i << " ";
    std::cout << "}" << std::endl;
  }


  /*
   *  Print the marked states
   */
  std::cout << "marked states:" << std::endl;
  for (auto states: marked_states) {
    std::cout << "states { ";
    for (auto s: states.second)
      std::cout << s + 1 << " ";
    std::cout << "} marks " << states.first << std::endl;
  }

  /*
   *  Print the start state
   */
  std::cout << "start state: " << start_state_id + 1 << std::endl;


  return dfa {
      indexed_transitions,
      start_state_id,
      accepting_states,
      marked_states
    };
}

}
