#ifndef _REGEX_LEXER_H
#define _REGEX_LEXER_H

#include <vector>
#include <string>
#include <algorithm>
#include "../token.hpp"
#include "alphabet.hpp"


template<typename token_type_t>
class source {
 public:
  typedef token_type_t token_type;
  typedef typename token_type::symbol_type symbol_type;
  
  virtual ~source() {}

  virtual bool eof() = 0;
  virtual char get() = 0;
  virtual char peek() = 0;
  virtual void discard() = 0;
  virtual void discard(std::size_t length) = 0;
  virtual std::string extract() = 0;
  virtual std::string extract(std::size_t length) = 0;
  virtual token_type* build_token(symbol_type s) = 0;
  virtual token_type* build_token(symbol_type s, const std::string& v) = 0;
  virtual token_type* build_token(symbol_type s, std::size_t length) = 0;
  virtual std::string render_current_coordinates() = 0;
};


template<typename token_type_t>
class string_source: public source<token_type_t> {
 public:
  using typename source<token_type_t>::token_type;
  typedef typename token_type::symbol_type symbol_type;

  explicit string_source(const std::string& s)
      : buffer(s), p1(buffer.begin()), p2(buffer.begin()),
        start_line(0), start_column(0),
        current_line(0), current_column(0) {}
  
  virtual ~string_source() {}

  void set(const std::string& s) {
    buffer = s;
    p1 = buffer.begin();
    p2 = buffer.begin();
  }

  virtual bool eof() {
    return p2 == buffer.end();
  }

  virtual char get() {
    increment_coordinates();
    return *(p2++);
  }

  virtual char peek() {
    return *p2;
  }

  virtual void discard() {
    p1 = p2;

    start_line = current_line;
    start_column = current_column;
  }

  virtual void discard(std::size_t length) {
    p2 = p1;
    
    current_line = start_line;
    current_column = start_column;

    for (std::size_t i(0); i < length; ++i) {
      increment_coordinates();
      ++p2;
    }

    discard();
  }

  virtual std::string extract() {
    const std::string result(p1, p2);

    discard();

    return result;
  }

  virtual std::string extract(std::size_t length) {
    const std::string result(p1, p1 + length);

    discard(length);

    return result;
  }

  virtual token_type* build_token(symbol_type s) {
    return build_token(s, std::string(p1, p2));
  }

  virtual token_type* build_token(symbol_type s, std::size_t length) {
    token_type* result(new string_token<symbol_type>(s, std::string(p1, p1 + length),
                                                     start_line, start_column));
    discard(length);

    return result;
  }

  virtual std::string render_current_coordinates() {
    return source_coordinate_range(start_line, start_column, 0).render();
  }

  virtual token_type* build_token(symbol_type s, const std::string& v) {
    token_type* result(new string_token<symbol_type>(s, v,
                                                     start_line, start_column));
    discard();

    return result;
  }

 private:
  std::string buffer;
  std::string::iterator p1, p2;

  int start_line, start_column;
  int current_line, current_column;

 private:
  void increment_coordinates() {
    if (*p2 == '\n') {
      ++current_line;
      current_column = 0;
    } else {
      ++current_column;
    }
  }
};

template<typename token_type_t>
class file_source: public source<token_type_t> {
 public:
  using typename source<token_type_t>::token_type;
  typedef typename token_type::symbol_type symbol_type;

  file_source(std::istream* stream, const std::string& f)
      : file(stream),
        p1(buffer.begin()), p2(buffer.begin()),
        file_name(f),
        start_line(1), start_column(0),
        current_line(0), current_column(0),
        chunk_size(1) {
    fill();
  }

  file_source()
      : file(nullptr),
        p1(buffer.begin()), p2(buffer.begin()),
        file_name(""),
        start_line(1), start_column(0),
        current_line(0), current_column(0),
        chunk_size(1) {
    fill();
  }
    
  
  virtual ~file_source() {}

  void set_file(std::istream* stream, const std::string& f) {
    buffer.clear();
    p1 = buffer.begin();
    p2 = buffer.begin();

    file_name = f;
    file = stream;

    start_line = 1;
    start_column = 0;

    current_line = 1;
    current_column = 0;

    fill();
  }

  virtual bool eof() {
    return (not file) or (file->eof() and p2 == buffer.end());
  }
  
  virtual char get() {
    increment_coordinates();
    const char c(*(p2++));
    if (p2 == buffer.end())
      fill();
    return c;
  }

  virtual char peek() {
    return *p2;
  }

  virtual void discard() {
    p1 = p2;

    start_line = current_line;
    start_column = current_column;
  }

  virtual void discard(std::size_t length) {
    p2 = p1;

    current_line = start_line;
    current_column = start_column;
    
    for (std::size_t i(0); i < length; ++i) {
      increment_coordinates();
      ++p2;
    }

    discard();
  }

  virtual std::string extract() {
    const std::string result(p1, p2);

    discard();

    return result;
  }

  virtual std::string extract(std::size_t length) {
    const std::string result(p1, p1 + length);

    discard(length);

    return result;
  }

  virtual token_type* build_token(symbol_type s) {
    return build_token(s, std::string(p1, p2));
  }

  virtual token_type* build_token(symbol_type s, std::size_t length) {
    token_type* tmp(new file_token<symbol_type>(s, std::string(p1, p1 + length),
                                                file_name, start_line, start_column));
    discard(length);

    return tmp;
  }
  
  virtual token_type* build_token(symbol_type s, const std::string& v) {
    token_type* tmp(new file_token<symbol_type>(s, v,
                                                file_name, start_line, start_column));

    discard();
    
    return tmp;
  }

  virtual std::string render_current_coordinates() {
    return file_source_coordinate_range(file_name, start_line, start_column, 0).render();
  }
  
 private:
  std::istream* file;
  std::vector<char> buffer;
  std::vector<char>::iterator p1, p2;

  std::string file_name;

  std::size_t start_line, start_column;
  std::size_t current_line, current_column;

  std::size_t chunk_size;
  
 private:
  void increment_coordinates() {
    if (*p2 == '\n') {
      ++current_line;
      current_column = 0;
    } else {
      ++current_column;
    }
  }
  
  void fill() {
    if (file) {
      const std::size_t p2_distance(std::distance(p1, p2));
      buffer.erase(buffer.begin(), p1);
      buffer.resize(buffer.size() + chunk_size);
      p1 = buffer.begin();
      p2 = p1 + p2_distance;
    
      file->read(&(*p2), chunk_size);
      std::size_t actual_chunk_size(file->gcount());
      if (actual_chunk_size < chunk_size)
	buffer.resize(buffer.size() - chunk_size + actual_chunk_size);
    }
  }
};


template<typename token_type_t>
class file_stack_source: public source<token_type_t> {
 public:
  using typename source<token_type_t>::token_type;
  typedef typename token_type::symbol_type symbol_type;

  file_stack_source(file_source<token_type_t>* fs)
      : files(1, fs) {}
  virtual ~file_stack_source() {}
  
  virtual bool eof() {
    if (files.size() == 1)
      return files.back()->eof();
    
    if (files.back()->eof()) {
      files.pop_back();
      return eof();
    }

    return false;
  }
  
  virtual char get() {
    if (not eof())
      return files.back()->get();
    else
      throw std::string("Ohh noooo, eof reached!");
  }
  
  virtual char peek() {
    return files.back()->peek();
  }
  
  virtual void discard() {
    files.back()->discard();
  }
  
  virtual void discard(std::size_t length) {
    files.back()->discard(length);
  }

  virtual std::string extract() {
    return files.back()->extract();
  }

  virtual std::string extract(std::size_t length) {
    return files.back()->extract(length);
  }

  virtual token_type* build_token(symbol_type s) {
    return files.back()->build_token(s);
  }
  
  virtual token_type* build_token(symbol_type s, std::size_t length) {
    return files.back()->build_token(s, length);
  }
  
  virtual token_type* build_token(symbol_type s, const std::string& v) {
    return files.back()->build_token(s, v);
  }
  
  virtual std::string render_current_coordinates() {
    return files.back()->render_current_coordinates();
  }

  void push_source(file_source<token_type>* fs) {
    files.push_back(fs);
  }
  
 private:
  std::vector<file_source<token_type_t>* > files;
};


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
