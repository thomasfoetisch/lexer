#ifndef REGEX_LEXER_BUILDER_H
#define REGEX_LEXER_BUILDER_H

#include "regex_lexer.hpp"


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

#endif /* REGEX_LEXER_BUILDER_H */
