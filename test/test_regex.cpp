
#include <iostream>
#include <sstream>

#include "regex.hpp"


void test_lexer_string(const std::string& arg) {
  regex::lexer_string lex(arg);

  regex::alphabet alphabet;
  regex::lexer_string::token_type tok;
  do {
    if (lex.read_token(tok)) {
      std::cout << "token: " << alphabet.name(tok.symbol)
                << " ('" << tok.value << "')"
                << std::endl;
    } else {
      std::cout << "An error occured." << std::endl;
      tok.symbol = regex::alphabet::symbol_type::end_of_input;
    }
  } while (tok.symbol != regex::alphabet::symbol_type::end_of_input);
}


void test_lexer_stream(const std::string& arg) {
  std::istringstream iss(arg);
  regex::lexer_istream lex(iss);
  
  regex::alphabet alphabet;
  regex::lexer_istream::token_type tok;
  do {
    if (lex.read_token(tok)) {
      std::cout << "token: " << alphabet.name(tok.symbol)
                << " ('" << tok.value << "')"
                << std::endl;
    } else {
      std::cout << "An error occured." << std::endl;
      tok.symbol = regex::alphabet::symbol_type::end_of_input;
    }
  } while (tok.symbol != regex::alphabet::symbol_type::end_of_input);
}


void test_stream_input(const std::string& arg) {
  std::istringstream iss(arg);
  regex::lexer_istream source(iss);

  while (not source.eof())
    std::cout << source.get();
  std::cout << std::endl;
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "One argument required. Aborting." << std::endl;
    return 0;
  }

  std::string argument(argv[1]);
  test_stream_input(argument);
  test_lexer_string(argument);
  test_lexer_stream(argument);

  return 0;
}
