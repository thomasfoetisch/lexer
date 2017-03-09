#ifndef TOKEN_BUFFER_H
#define TOKEN_BUFFER_H

#include <string>
#include <deque>


template<typename token_source>
class token_buffer {
 public:
  typedef typename token_source::token_type token_type;
  typedef typename token_type::symbol_type symbol_type;
  
  explicit token_buffer(token_source* s): src(s) {
    fill(1);
  }

  void clear() {
    buffer.clear();
  }
  
  bool match(symbol_type symbol) {
    if (buffer.front()->match(symbol)) {
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
  token_type* look_ahead(std::size_t n) {
    fill(n + 1);
    return buffer[n];
  }

 private:
  token_source* src;
  std::deque<token_type*> buffer;

  void fill(std::size_t n) {
    while (buffer.size() < n) {
      token_type* tmp(src->read_token());
      if (not tmp)
        throw std::string("lexing failed.");
      buffer.push_back(tmp);
    }
  }
};

#endif /* TOKEN_BUFFER_H */
