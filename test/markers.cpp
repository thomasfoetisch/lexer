
#include <string>
#include <vector>

#include "../regex/lexer_string.hpp"
//#include "../token_stream.hpp"
#include "../regex/ast/ast.hpp"
#include "../regex/dfa.hpp"
#include "../regex/build_dfa.hpp"
#include "../regex/match.hpp"
#include "../regex/parser.hpp"

#include "../stream.hpp"

template<typename token_type>
class token_stream: public stream<token_type> {
 public:
  typedef typename token_type::symbol_type symbol_type;


  token_stream(buffered_stream<char>* s): char_source(s) {}
  
  void emit(symbol_type s, const std::string& re) {
    /*
     *  Parse the regex
     */
    regex::lexer_string lex(re);
    token_stream<regex::lexer_string> tok_stream(&lex);
    regex::regex_parser p;

    regex::ast::node* root(p.parse_re<regex::lexer_string>(&tok_stream));


    /*
     *  Record the associated symbol
     */
    symbols.push_back(s);
    const std::size_t symbol_id(symbols.size());


    /*
     *  Append the alternative to the set of lexems
     */
    append(new regex::ast::alt(root, new regex::ast::marker(symbol_id)));
  }
  
  void skip(const std::string& re) {
    /*
     *  Parse the regex
     */
    regex::lexer_string lex(re);
    token_stream<regex::lexer_string> tok_stream(&lex);
    regex_parser p;

    regex::ast::node* root(p.parse_re<regex::lexer_string>(&tok_stream));


    /*
     *  Populate the deterministic finite automaton context
     */
    regex::dfa_context c;
    root->initialize_positions(1);
    root->build_last_pos();
    root->build_first_pos();
    root->populate(c);

    
    /*
     *  Build the automaton
     */
    skip_automaton = regex::build_dfa(c, root);
  }


  void compile() {
    /*
     *  Populate the deterministic finite automaton context
     */
    regex::dfa_context c;
    root->initialize_positions(1);
    root->build_last_pos();
    root->build_first_pos();
    root->populate(c);

    
    /*
     *  Build the automaton
     */
    lexer_automaton = regex::build_dfa(c, regex_ast);
  }
  
  void clear() {
    symbols.clear();

    delete regex_ast;
    regex_ast = NULL;
  }

  bool eos() {
    return char_source->eos();
  }
  
  token_type get() {
    std::size_t current_state(lexer_automaton.start_state);
    char symbol(char_source->get());
  
    bool abort(false);
    std::size_t n(0);
    std::size_t longest_match(0);
    std::size_t longest_match_symbol_id(0);
    while (not abort) {
      std::cout << "current state: " << current_state + 1 << std::endl;
      if (char_source->eos()) {
        abort = true;
      } else {
        const std::size_t next_state(lexer_automaton.transitions[current_state][*symbol]);
        if (next_state == 0) {
          abort = true;
        } else {
          current_state = next_state - 1;
          symbol = char_source->get();
          ++n;

          /*
           *  Check if the current state is marked
           */
          auto m(lexer_automaton.marked_states.find(current_state));
          if (m != lexer_automaton.marked_states.end()
              and m->second.size()) {
            longest_match_symbol_id = *m->second.begin();
            longest_match = n;
          }
        }
      }
    }
  
    return token_type(symbols[longest_match_symbol_id],
                      char_source->extract<std::string>(longest_match));
  }
  
 private:
  void append(regex::ast::node* n) {
    if (not regex_ast) {
      regex_ast = n;
    } else {
      regex_ast = new regex::ast::alt(regex_ast, n);
    }
  }

  buffered_stream<char>* char_source;
  
  regex::ast::node* regex_ast;

  std::vector<symbol_type> symbols;

  regex::dfa skip_automaton, lexer_automaton;
};


namespace lemmin {
enum class: int symbol {
  number, identifier, comma
};
}

int main(int argc, char *argv[]) {
  typedef token<lemmin::symbol> token_type;
  typedef token_stream<token_type> token_stream_type;

  std::ifstream source_file("source.src", std::ios::in);
  istream_stream char_source(&source_file);
  buffered_stream<char> buffered_char_source(&char_source);
  
  token_stream<token_type> lexer(&buffered_char_source);
  lexer.emit("", lemmin::symbol::number);
  lexer.emit("", lemmin::symbol::identifier);
  lexer.emit(",", lemmin::symbol::comma);
  lexer.skip("\r\t\n ");
  lexer.compile();

      
  while (not lexer->eos()) {
    const token_type tmp(lexer->get());
    
    std::cout << tmp.value << std::endl;
  }
  
  return 0;
}

/*
int main(int argc, char *argv[]) {
  try {
    regex::lexer_string lex;
    token_stream<regex::lexer_string> tok_stream(&lex);

    regex::parser p;

    lex.set("(a|b)*abb");
    tok_stream.clear();
    regex::ast::node* left_leg(new regex::ast::cat(p.parse_re<regex::lexer_string>(&tok_stream),
                                                    new regex::ast::marker(1)));

    lex.set("c*abb");
    tok_stream.clear();
    regex::ast::node* right_leg(new regex::ast::cat(p.parse_re<regex::lexer_string>(&tok_stream),
                                                    new regex::ast::marker(2)));
    
    
    regex::ast::node* root(new regex::ast::cat(new regex::ast::cat(left_leg, right_leg),
                                               new regex::ast::end_marker()));

    regex::dfa_context c;
    root->initialize_positions(1);
    root->build_last_pos();
    root->build_first_pos();

    root->populate(c);

    root->print();

    c.print();
    
    regex::dfa automaton(regex::build_dfa(c, root));

    std::cout << "does '" << argv[1] << "' matches /" << "(a|b)*abbc*abb" << "/? "
              << regex::match(automaton, argv[1]) << std::endl;
  }
  catch(const std::string& e) {
    std::cout << e << std::endl;
  }

  
  return 0;
}
*/
