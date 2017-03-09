#ifndef LEXER_ISTREAM_H
#define LEXER_ISTREAM_H

#include <istream>
#include <vector>
#include "lexer.hpp"

namespace regex {


class lexer_istream: public lexer {
 public:
  virtual bool eof() const {
    return eof_bit and (iterator == buffer.end());
  }

  virtual char get() {
    if (iterator == buffer.end() and not eof_bit)
      fill();
    return *(iterator++);
  }

  explicit lexer_istream(std::istream& s)
      : stream(s),
        eof_bit(false),
        buffer_size(4096) {
    buffer.reserve(buffer_size);
    fill();
  }

 private:
  std::istream& stream;
  std::vector<char> buffer;
  std::vector<char>::const_iterator iterator;

  std::size_t buffer_size;
  bool eof_bit;

  void fill() {
    buffer.resize(buffer_size);
    stream.read(&buffer[0], buffer_size);
    if (stream.gcount() != buffer_size) {
      buffer.resize(stream.gcount());
      eof_bit = true;
    }
    iterator = buffer.begin();
  }
};


}  /* namespace regex */

#endif /* LEXER_ISTREAM_H */
