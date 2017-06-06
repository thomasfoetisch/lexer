#include <iostream>
#include <string>
#include <fstream>

#include "../src/lexer.hpp"

/*
 * Grammar of the description of an electrical circuit made of
 * resistors and voltage sources:
 * 
 * <graph> = EPSILON | <link> <graph>,
 * <link> = <elem-type> LP <value> RP FROM NODE-ID TO NODE-ID COLON,
 * <value> = REAL <unit>,
 * <unit> = MULTIPLIER UNIT-ID | UNIT-ID,
 */

enum class symbol {
  graph, link, elem_type,
  lp, rp, value, from, to,
  node_id, semicolon,
  real, unit, multiplier, unit_id,
  eoi
};

std::ostream& operator<<(std::ostream& stream, symbol s) {
  switch (s) {
  case symbol::graph:
    stream << "graph";
    break;
    
  case symbol::link:
    stream << "link";
    break;
    
  case symbol::elem_type:
    stream << "elem_type";
    break;
    
  case symbol::lp:
    stream << "lp";
    break;
    
  case symbol::rp:
    stream << "rp";
    break;
    
  case symbol::value:
    stream << "value";
    break;
    
  case symbol::from:
    stream << "from";
    break;
    
  case symbol::to:
    stream << "to";
    break;
    
  case symbol::node_id:
    stream << "node_id";
    break;
    
  case symbol::semicolon:
    stream << "semicolon";
    break;
    
  case symbol::real:
    stream << "real";
    break;
    
  case symbol::unit:
    stream << "unit";
    break;
    
  case symbol::multiplier:
    stream << "multiplier";
    break;

  case symbol::unit_id:
    stream << "unit_id";
    break;

  case symbol::eoi:
    stream << "eoi";
    break;
  }
  
  return stream;
}

typedef symbol symbol_type;
typedef token<symbol_type> token_type;

regex_lexer<token_type> build_lexer() {
  regex_lexer_builder<token_type> rlb(symbol::eoi);
  {
    rlb.emit(symbol::elem_type, "(Resistor)|(Voltage)");
    rlb.emit(symbol::from, "from");
    rlb.emit(symbol::to, "to");
    rlb.emit(symbol::multiplier, "(Kilo)|(Mega)|(Giga)|(Milli)|(Micro)|(Nano)");
    rlb.emit(symbol::unit_id, "(Volt)|(Ohm)");
    rlb.emit(symbol::semicolon, ";");
    rlb.emit(symbol::real,
	     "[+-]?"
	     "((\\.\\d+)|(\\d+\\.)|(\\d+\\.\\d+)|(\\d+))"
	     "([eE][+-]?\\d+)?");
    rlb.emit(symbol::node_id, "[\\l][\\l\\d_]*");

    rlb.emit(symbol::lp, "\\(");
    rlb.emit(symbol::rp, "\\)");

    rlb.skip("(\\s|(//[^\\n]*\\n))*");
  }

  return rlb.build();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "expecting 1 argument." << std::endl;
    return 1;
  }

  regex_lexer<token_type> lex(build_lexer());

  std::ifstream f(argv[1], std::ios::in);
  file_source<token_type> fs(&f, argv[1]);
  lex.set_source(&fs);

  token_type* t(lex.get());
  while (t->symbol != symbol::eoi) {
    std::cout << *t << std::endl;
    delete t;
    t = nullptr;

    t = lex.get();
  }

  delete t;
  t = nullptr;
  
  return 0;
}
