#ifndef _LEXER_H_
#define _LEXER_H_

#include <vector>
#include <string>

#include "regex/lexer.hpp"
#include "regex/parser.hpp"

#include "regex/ast/ast.hpp"
#include "regex/dfa.hpp"
#include "regex/build_dfa.hpp"
#include "regex/match.hpp"

#include "token_buffer.hpp"



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
      std::size_t next_state(skipper_automaton.transitions[current_state][symbol]);
      if (next_state) {
        src.back()->get();
        current_state = next_state - 1;
      } else {
        abort = true;
      }
    }

    src.back()->discard();
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
      throw std::string("regex_lexer::get(): "
                        "input error near " + src.back()->render_current_coordinates());
  }
  
 private:
  std::vector<source<token_type>*> src;
  
  regex::dfa skipper_automaton, lexer_automaton;
  symbol_type end_of_input_symbol;
  std::map<std::size_t, symbol_type> s_map;
};


template<typename token_type>
class regex_lexer_builder {
 public:
  typedef typename token_type::symbol_type symbol_type;

  explicit regex_lexer_builder(symbol_type eoi_symbol)
      : regex_ast(NULL), end_of_input_symbol(eoi_symbol) {}
  
  void emit(symbol_type s, const std::string& re) {
    /*
     *  Parse the regex
     */
    string_source<token<regex::symbol> > ss(re);
    regex::lexer lex(&ss);
    token_buffer<regex::lexer> tb(&lex);
    regex::parser<token_buffer<regex::lexer> > p;

    regex::ast::node* root(p.parse(&tb));


    
    /*
     *  Record the associated symbol
     */
    symbols.push_back(s);
    const std::size_t symbol_id(symbols.size());


    /*
     *  Append the alternative to the set of lexems
     */
    append(new regex::ast::cat(root, new regex::ast::marker(symbol_id)));
  }
  
  void skip(const std::string& re) {
    /*
     *  Parse the regex
     */
    string_source<token<regex::symbol> > ss(re);
    regex::lexer lex(&ss);
    token_buffer<regex::lexer> tb(&lex);
    regex::parser<token_buffer<regex::lexer>> p;
    regex::ast::node* root(new regex::ast::cat(p.parse(&tb),
                                               new regex::ast::marker(0)));


    /*
     *  Populate the deterministic finite automaton context
     */
    regex::dfa_context c;
    root->initialize_positions(1);
    root->build_last_pos();
    root->build_first_pos();
    root->populate(c);

    //root->print();

    /*
     *  Build the automaton
     */
    skip_automaton = regex::build_dfa(c, root);

    delete root;
    root = NULL;
  }

  regex_lexer<token_type> build() {
    compile();

    regex_lexer<token_type> result(lexer_automaton,
                                   skip_automaton,
                                   symbols,
                                   end_of_input_symbol);

    clear();
    
    return result;
  }

 private:
  regex::ast::node* regex_ast;
  regex::dfa skip_automaton, lexer_automaton;
  std::vector<symbol_type> symbols;
  symbol_type end_of_input_symbol;


 private:
  void append(regex::ast::node* n) {
    if (not n)
      throw std::string("trying to append a NULL node, forbidden.");
    
    if (not regex_ast) {
      regex_ast = n;
    } else {
      regex_ast = new regex::ast::alt(regex_ast, n);
    }
  }

  void compile() {
    /*
     *  Populate the deterministic finite automaton context
     */
    regex::dfa_context c;
    regex_ast->initialize_positions(1);
    regex_ast->build_last_pos();
    regex_ast->build_first_pos();
    regex_ast->populate(c);

    //regex_ast->print();

    
    /*
     *  Build the automaton
     */
    lexer_automaton = regex::build_dfa(c, regex_ast);
  }
  
  void clear() {
    symbols.clear();
    skip_automaton.clear();

    delete regex_ast;
    regex_ast = NULL;
  }
};


#endif /* _LEXER_H_ */
