
#include <fstream>
#include <string>
#include "../regex/lexer.hpp"


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
  }
  
  return stream;
}


int main(int argc, char** argv) {
  typedef dummy_alphabet symbol_type;
  typedef file_token<symbol_type> token_type;
  
  std::ifstream f("token_sample", std::ios::in);
  file_source<token_type> fs(&f, "token_sample");

  while (not fs.eof()) {
    if (fs.get() == '\n') {
      fs.discard();
    } else {
      token_type* t(fs.build_token(dummy_alphabet::integer));
      std::cout << *t << std::endl;
      delete t;
    }
  }
  
  return 0;
}
