#ifndef TOKEN_H
#define TOKEN_H

#include "source_coordinate_range.hpp"

template<typename symbol_t>
struct token {
  typedef symbol_t symbol_type;
  
  token(symbol_type s, const std::string& v): symbol(s), value(v) {}
  virtual ~token() {}
  
  const symbol_type symbol;
  const std::string value;

  bool match(symbol_type s) {
    return s == symbol;
  }

  virtual std::string render_coordinates() const = 0;
  virtual std::string render() const {
    std::ostringstream oss;
    oss << symbol << "(" << value << " at " << render_coordinates() << ")";
    return oss.str();
  }
  virtual const source_coordinate_range* get_coordinates() const = 0;

  virtual token<symbol_type>* copy() const = 0;
};


template<typename symbol_type>
struct repl_token: public token<symbol_type> {
  repl_token(symbol_type s, const std::string& v,
             int hist_idx, int line_num, int col_num)
      : token<symbol_type>(s, v),
        coordinates(hist_idx, line_num, col_num, v.size()) {}
  virtual ~repl_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;
  
  virtual std::string render_coordinates() const {
    return coordinates.render();
  }

  virtual const repl_source_coordinate_range* get_coordinates() const {
    return &coordinates;
  }

  virtual repl_token<symbol_type>* copy() const {
    return new repl_token<symbol_type>(*this);
  }
      
 private:
  repl_source_coordinate_range coordinates;
};

template<typename symbol_type>
struct string_token: public token<symbol_type> {
  string_token(symbol_type s, const std::string& v,
               int line_num, int col_num)
      : token<symbol_type>(s, v),
        coordinates(line_num, col_num, v.size()) {}
  virtual ~string_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;

  virtual std::string render_coordinates() const {
    return coordinates.render();
  }

  virtual const source_coordinate_range* get_coordinates() const {
    return &coordinates;
  }

  virtual string_token<symbol_type>* copy() const {
    return new string_token<symbol_type>(*this);
  }

 private:
  source_coordinate_range coordinates;
};


template<typename symbol_type>
struct file_token: public token<symbol_type> {
  file_token(symbol_type s, const std::string& v,
             const std::string& f, int line_num, int col_num)
    : token<symbol_type>(s, v),
      coordinates(f, line_num, col_num, v.size()) {}
  virtual ~file_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;

  virtual std::string render_coordinates() const {
    return coordinates.render();
  }

  virtual const source_coordinate_range* get_coordinates() const {
    return &coordinates;
  }

  virtual file_token<symbol_type>* copy() const {
    return new file_token<symbol_type>(*this);
  }

 private:
  file_source_coordinate_range coordinates;
};


template<typename symbol_type>
std::ostream& operator<<(std::ostream& stream, const token<symbol_type>& t) {
  stream << t.render();
  return stream;
}

#endif /* TOKEN_H */
