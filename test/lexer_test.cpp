
#include <fstream>
#include <string>

#include "../src/lexer.hpp"

enum class symbol {
  int_number, fp_number, identifier, lit_string,
  doc_block, doc_line,
  rp, lp, lb, rb,
  eoi
};

std::ostream& operator<<(std::ostream& stream, symbol s) {
  switch (s) {
    case symbol::int_number:
      stream << "integer_number";
      break;

    case symbol::fp_number:
      stream << "floating_point_number";
      break;
      
    case symbol::lit_string:
      stream << "string";
      break;

    case symbol::eoi:
      stream << "end_of_input";
      break;

    case symbol::identifier:
      stream << "identifier";
      break;

    case symbol::lp:
      stream << "left_parent";
      break;

    case symbol::rp:
      stream << "right_parent";
      break;

    case symbol::lb:
      stream << "left_bracket";
      break;

    case symbol::rb:
      stream << "right_bracket";
      break;

    case symbol::doc_block:
      stream << "doc_block";
      break;

    case symbol::doc_line:
      stream << "doc_line";
      break;
      
    default:
      throw std::string("undefined token name");
      break;
  }

  return stream;
}

int main(int argc, char** argv) {
  if (argc == 2) {
    std::string s(argv[1]);
    //s = "-0.02348e-45\\nabdcd";
    /*s = " [] () -12.43e-44 +12.e+3 34e2 1119 .55e-17 -12345 \n"
                "abcd_23 \"1234\" \n"
                "#un commentaire\n"
                "( [ ] )";
    std::cout << s << std::endl;
    */
  
    try {
      typedef symbol symbol_type;
      typedef token<symbol_type> token_type;

  
      regex_lexer_builder<token_type> rlb(symbol::eoi);
      {
        rlb.emit(symbol::fp_number,
                 "[+-]?"
                 "((\\.\\d+)|(\\d+\\.)|(\\d+\\.\\d+)|(\\d+))"
                 "([eE][+-]?\\d+)?");
        rlb.emit(symbol::int_number, "[+-]?\\d+");
        rlb.emit(symbol::identifier, "[\\l\\u_]\\w*");
        rlb.emit(symbol::lit_string, "'[^']*'");
        rlb.emit(symbol::doc_block, "##((#[^#])|[^#])*##");
        rlb.emit(symbol::doc_line, "#[^\\n]*\\n");
        rlb.emit(symbol::lp, "\\(");
        rlb.emit(symbol::rp, "\\)");
        rlb.emit(symbol::lb, "\\[");
        rlb.emit(symbol::rb, "\\]");
    
        rlb.skip("(\\s|(;[^\\n]*\\n))*");
      }
    
      regex_lexer<token_type> lex(rlb.build());

      std::ifstream f(s.c_str(), std::ios::in);
      if (f.good()) {
        file_source<token_type> fs(&f, s);
        lex.set_source(&fs);


        token_type* t(lex.get());
        while (t->symbol != symbol::eoi) {
          std::cout << *t << std::endl;
          delete t;
          t = NULL;
      
          t = lex.get();
        }
    
        delete t;
        t = NULL;
      } else {
        std::cout << "Unable to read file " << s << std::endl;
      }
    }
    catch(const std::string& e) {
      std::cout << e << std::endl;
    }
  } else {
    std::cout << "Usage: " << argv[0] << " <code_sample>" << std::endl;
  }
  return 0;
}
