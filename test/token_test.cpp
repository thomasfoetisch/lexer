
#include "../src/token.hpp"
#include <iostream>


enum class dummy_alphabet {
  integer, comma
};

std::ostream& operator<<(std::ostream& stream, dummy_alphabet s) {
  switch (s) {
    case dummy_alphabet::integer:
      stream << "integer";
      break;

    case dummy_alphabet::comma:
      stream << "comma";
      break;
      
    default:
      break;
  };
  
  return stream;
}

int main(int argc, char** argv) {

  typedef dummy_alphabet symbol_type;
  typedef repl_token<symbol_type> token_type;

  token_type t(dummy_alphabet::integer, "42429", 1, 1, 3);

  std::cout << t.render_coordinates() << std::endl;

  std::cout << t << std::endl;
  
  return 0;
}
