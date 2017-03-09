#ifndef LEXER_STRING_H
#define LEXER_STRING_H

#include <string>
#include "lexer.hpp"

namespace regex {

class lexer_string: public lexer {
 public:
  virtual bool eof() const {
    return iterator == string.end();
  }
  virtual char get() {
    return *(iterator++);
  }

  explicit lexer_string(const std::string& s)
      : string(s),
        iterator(string.begin()) {}

  lexer_string(): string(),
                  iterator(string.begin()) {}
    
  
  void set(const std::string& s) {
    string = s;
    iterator = string.begin();
  }
  
 private:
  std::string string;
  std::string::const_iterator iterator;
};


}  /* namespace regex */

#endif /* LEXER_STRING_H */
