#ifndef _REGEX_LEXER_H
#define _REGEX_LEXER_H

#include "source.hpp"


namespace regex {

class lexer {
 public:
  typedef token<regex::symbol> token_type;
  typedef regex::symbol symbol_type;

  explicit lexer(source<token_type>* s)
      : src(s),
        escape_sequences({'0', 'a', 'b', 't', 'n', 'v', 'f', 'r',  'e',
                          '[', ']', '(', ')', '|', '*', '.', '\\', '+', '?',
                          'l', 'u', 'd', 's', 'w'}) {}

  virtual ~lexer() {}

  void set_source(source<token_type>* s) {
    src = s;
  }

  token_type* read_token() {
    if (src->eof()) {
      return src->build_token(symbol::end_of_input, "");
    } else {
      switch (src->get()) {
        case '\\':
          if (not read_escaped_sequence())
            return NULL;
          return src->build_token(symbol::char_class);
          break;

        case '[':
          if (read_character_class())
            return src->build_token(symbol::char_class);
          return NULL;
          break;

        case '*': return src->build_token(symbol::star);
        case '+': return src->build_token(symbol::star);
        case '?': return src->build_token(symbol::star);
        case '|': return src->build_token(symbol::pipe);
        case '(': return src->build_token(symbol::lp);
        case ')': return src->build_token(symbol::rp);

        default: return src->build_token(symbol::char_class);
      }
    }
  }

 protected:
  bool read_character_class() {
    bool done(false);

    while (not done) {
      if (src->eof())
        return false;
      
      switch (src->get()) {
        case '\\':
          if (not read_escaped_sequence())
            return false;
          break;

        case ']':
          done = true;
          break;

        default:
          break;
      }
    }
    return true;
  }

  bool read_escaped_sequence() {
    if (src->eof())
      return false;

    std::vector<char>::iterator seq(std::find(escape_sequences.begin(),
                                              escape_sequences.end(),
                                              src->get()));

    return seq != escape_sequences.end();
  }

 private:
  source<token_type>* src;
  std::vector<char> escape_sequences;
};

}  /* namespace regex */

#endif /* LEXER_H */
