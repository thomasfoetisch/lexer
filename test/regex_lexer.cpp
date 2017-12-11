
#include <iostream>
#include <string>
#include "../src/regex/lexer.hpp"


int main(int argc, char** argv) {
  typedef regex::lexer::symbol_type symbol_type;
  typedef regex::lexer::token_type token_type;

  std::string s("[0-9]* \\n ab(c|d)*[^#\\]]");

  string_source<token_type> ss(s);
  regex::lexer lex(&ss);

  token_type* t(NULL);
  while ((t = lex.read_token())
         and
         (not t->match(regex::symbol::end_of_input))) {
    std::cout << *t << std::endl;
    
    delete t;
    t = NULL;
  }
  delete t;
  t = NULL;

  return 0;
}
