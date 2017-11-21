#include <iostream>

//#include <lexer/lexer.hpp>
#include "../src/lexer.hpp"

enum class symbol {number, comma, eoi};
std::ostream& operator<<(std::ostream& stream, const symbol& s) {
  switch (s) {
  case symbol::number: stream << "<number>"; break;
  case symbol::comma: stream << "<comma>"; break;
  case symbol::eoi: stream << "<eoi>"; break;
  }
  return stream;
}

int main() {
  try {
    using token_type = token<symbol>;

    regex_lexer_builder<token_type> rlb(symbol::eoi); {
      rlb.emit(symbol::number, "\\d+");
      rlb.emit(symbol::comma, ",");
      rlb.skip("[\n ]*");
    }
    regex_lexer<token_type> lex(rlb.build());
    string_source<token_type> src("12, 34, 88\n, 45");
    lex.set_source(&src);

    token_type* t(lex.get());
    while (t->symbol != symbol::eoi) {
      std::cout << t->symbol << "(" << t->value << ") at "
                << t->render_coordinates() << std::endl;
      delete t; t = lex.get();
    }
    delete t;
  }
  catch (const std::string& e) {
    std::cout << e << std::endl;
  }
    
  
  return 0;
}
