
#include "../src/regex/lexer.hpp"
#include "../src/regex/parser.hpp"

int main(int argc, char** argv) {
  typedef regex::lexer::token_type token_type;
  typedef token_type::symbol_type symbol_type;

  std::string s("[teu] \\n a(c|b)*");
  string_source<token_type> ss(s);
  regex::lexer lex(&ss);
  token_buffer<regex::lexer> tb(&lex);

  regex::parser<token_buffer<regex::lexer> > p;

  regex::ast::node* n(p.parse(&tb));

  n->print();

  delete n;
  n = NULL;
}
