#ifndef REGEX_LEXER_H
#define REGEX_LEXER_H

#include <vector>
#include <string>

#include "regex/lexer.hpp"
#include "regex/parser.hpp"

#include "regex/ast/ast.hpp"
#include "regex/dfa.hpp"
#include "regex/build_dfa.hpp"
#include "regex/match.hpp"

#include "token_buffer.hpp"


class lex_error {
public:
  lex_error(source_coordinate_range* c, const std::string& msg)
    : coordinates(c), msg(msg) {}

  const source_coordinate_range* get_coordinates() const { return coordinates; }
  const std::string& get_message() const { return msg; }
  
private:
  source_coordinate_range* coordinates;
  const std::string msg;
};


template<typename token_type>
class regex_lexer {
 public:
  typedef typename token_type::symbol_type symbol_type;

  regex_lexer(const regex::dfa& lexer,
              const regex::dfa& skip,
              const std::vector<symbol_type>& s,
              symbol_type eoi_symbol)
      : src(),
        skipper_automaton(skip),
        lexer_automaton(lexer),
        end_of_input_symbol(eoi_symbol) {
    for (auto mark: lexer.marked_states)
      for (auto state: mark.second) {
        auto tmp(s_map.find(state));
        if (tmp == s_map.end())
          s_map[state] = s[mark.first - 1];
        else if (tmp->second > s[mark.first - 1])
          s_map[state] = s[mark.first - 1];
      }

    /*
    for (auto s: s_map)
      std::cout << s.first << " -> " << s.second << std::endl;
    */
  }

  void set_source(source<token_type>* s) {
    src.push_back(s);
  }

  void skip() {
    bool abort(false);
    std::size_t current_state(skipper_automaton.start_state);
    
    while (not src.back()->eof() and not abort) {
      char symbol(src.back()->peek());
      if (symbol < 0)
	throw lex_error(src.back()->get_current_coordinates(),
                        "non ascii character");
      
      std::size_t next_state(skipper_automaton.transitions[current_state][symbol]);
      if (next_state) {
        src.back()->get();
        current_state = next_state - 1;
      } else {
        abort = true;
      }
    }

    skipped_characters = src.back()->extract();
  }

  const std::string& get_skipped_characters() const {
    return skipped_characters;
  }
  
  token_type* get() {
    while (true) {
      skip();
      if (src.back()->eof() and src.size() > 1)
        src.pop_back();
      else
        break;
    }
    
    std::size_t current_state(lexer_automaton.start_state);
  
    bool abort(false);
    std::size_t n(0);
    std::size_t longest_match(0);
    std::size_t longest_match_symbol_id(0);
    while (not abort and src.size()) {
      //std::cout << "current state: " << current_state + 1 << std::endl;
      if (src.back()->eof()) {
        abort = true;
      } else {
        char symbol(src.back()->peek());
        if (symbol < 0)
          throw lex_error(src.back()->get_current_coordinates(),
                          "non ascii character");
        
        const std::size_t next_state(lexer_automaton.transitions[current_state][symbol]);
        if (next_state == 0) {
          abort = true;
        } else {
          current_state = next_state - 1;
          src.back()->get();
          ++n;

          /*
           *  Check if the current state is marked
           */
          auto m(s_map.find(current_state));
          if (m != s_map.end()) {
            longest_match_symbol_id = m->first;
            longest_match = n;
          }
        }
      }
    }
    
    if (longest_match)
      return src.back()->build_token(s_map[longest_match_symbol_id], longest_match);
    else if (n == 0 and src.back()->eof())
      return src.back()->build_token(end_of_input_symbol, "");
    else
      throw lex_error(src.back()->get_current_coordinates(),
                      "unrecognized token");
  }

  void recover() {
    bool abort(false);
    const std::size_t start_state(skipper_automaton.start_state);
    
    while (not src.back()->eof() and not abort) {
      char symbol(src.back()->peek());
      if (symbol < 0) {
        src.back()->get();
      } else {
        std::size_t next_state(skipper_automaton.transitions[start_state][symbol]);
        if (next_state == 0) {
          src.back()->get();
        } else {
          abort = true;
        }
      }
    }
  }
  
 private:
  std::vector<source<token_type>*> src;
  
  regex::dfa skipper_automaton, lexer_automaton;
  symbol_type end_of_input_symbol;
  std::map<std::size_t, symbol_type> s_map;
  std::string skipped_characters;
};

#endif /* REGEX_LEXER_H */
