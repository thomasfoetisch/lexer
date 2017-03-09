
#include <string>
#include "../src/regex/lexer.hpp"


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
  typedef string_token<symbol_type> token_type;
  
  std::string s("123\n4\n5678\n90");
  string_source<token_type> ss(s);

  while (not ss.eof()) {
    if (ss.get() == '\n') {
      ss.discard();
    } else {
      token_type* t(ss.build_token(dummy_alphabet::integer));
      std::cout << *t << std::endl;
      delete t;
    }
  }
  
  return 0;
}
