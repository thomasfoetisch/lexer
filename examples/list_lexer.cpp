#include <iostream>
#include <fstream>

#include <lexer/lexer.hpp>


enum class symbol {
                   comma, number, boolean, string, eoi
};

std::string to_string(const symbol& s) {
  switch (s) {
  case symbol::comma: return "comma";
  case symbol::number: return "number";
  case symbol::boolean: return "boolean";
  case symbol::string: return "string";
  case symbol::eoi: return "eoi";
  default: return "unknown symbol";
  }
}

std::ostream& operator<<(std::ostream& stream, const symbol& s) {
  stream << to_string(s);
  return stream;
}

using token_type = token<symbol>;

regex_lexer<token_type> build_list_lexer() {
  regex_lexer_builder<token_type> rlb(symbol::eoi);
      
  rlb.emit(symbol::comma, ",");
  rlb.emit(symbol::number, "-?[1-9][0-9]*");
  rlb.emit(symbol::string, "\"[^\"]*\"");
  rlb.emit(symbol::boolean, "(true)|(false)");
  
  rlb.skip("[ \r\n\t]*");
      
  return rlb.build();
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 0;
  
  try {
    const std::string filename(argv[1]);
    std::ifstream f(filename.c_str(), std::ios::in);
  
    regex_lexer<token_type> l(build_list_lexer());

    file_source<token_type> fs(&f, filename);
    l.set_source(&fs);

    token_type* t(l.get());
    while (t->symbol != symbol::eoi) {
      std::cout << t->symbol << "(" << t->value << ") at "
                << t->render_coordinates() << std::endl;
      delete t; t = l.get();
    }
    delete t; t = nullptr;
  }
  catch (const std::string& e) {
    std::cout << e << std::endl;
  }
  
  return 0;
}
