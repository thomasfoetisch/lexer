
#include <iostream>

#include "../src/token.hpp"


enum class symbol {
  integer, comma
};

std::ostream& operator<<(std::ostream& stream, symbol s) {
  switch (s) {
  case symbol::integer: stream << "integer"; break;
  case symbol::comma: stream << "comma"; break;
    
  default:
    break;
  };
  
  return stream;
}

int main(int argc, char** argv) {

  typedef symbol symbol_type;
  typedef repl_token<symbol_type> token_type;

  token_type t(symbol::integer, "42429", 1, 1, 3);

  std::cout << t.render_coordinates() << std::endl;
  std::cout << t << std::endl;
  
  return 0;
}
