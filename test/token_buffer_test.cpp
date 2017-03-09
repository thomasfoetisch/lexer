
#include "../src/regex/lexer.hpp"
#include "../src/token_buffer.hpp"

int main(int argc, char** argv) {
  typedef regex::lexer::token_type token_type;
  typedef token_type::symbol_type symbol_type;

  std::string s("[teu] \\n a(c|b)*");
  string_source<token_type> ss(s);
  regex::lexer lex(&ss);
  token_buffer<regex::lexer> tb(&lex);

  std::size_t i(0);
  while (not tb.look_ahead(i)->match(regex::symbol::end_of_input)) {
    std::cout << *tb.look_ahead(i) << std::endl;
    ++i;
  }
}
