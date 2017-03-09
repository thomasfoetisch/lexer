#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include <string>
#include <deque>
#include "token.hpp"

template<typename lexer_type>
class token_stream {
 public:
  typedef typename lexer_type::token_type token_type;
  typedef typename lexer_type::symbol_type symbol_type;
  
  explicit token_stream(lexer_type* lex_): lex(lex_) {
    fill(1);
  }

  void clear() {
    buffer.clear();
  }
  
  bool match(symbol_type symbol) {
    if (buffer.front().symbol == symbol) {
      consume();
      return true;
    } else {
      return false;
    }
  }
  void consume() {
    buffer.pop_front();
    fill(1);
  }
  token_type look_ahead(std::size_t n) {
    fill(n + 1);
    return buffer[n];
  }

 private:
  lexer_type* lex;
  std::deque<token_type > buffer;

  void fill(std::size_t n) {
    while (buffer.size() < n) {
      token_type tmp;
      if (not lex->read_token(tmp))
        throw std::string("lexing failed.");
      buffer.push_back(tmp);
    }
  }
};

#endif /* TOKEN_STREAM_H */
