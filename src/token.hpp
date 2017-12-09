#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <sstream>

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
  virtual std::string render_full() const {
    std::ostringstream oss;
    oss << symbol << "(" << value << " at " << render_coordinates() << ")";
    return oss.str();
  }

  virtual token<symbol_type>* copy() const = 0;
};


template<typename symbol_type>
struct repl_token: public token<symbol_type> {
  repl_token(symbol_type s, const std::string& v,
             int hist_idx, int line_num, int col_num)
      : token<symbol_type>(s, v),
        history_index(hist_idx),
        line_number(line_num),
        column_number(col_num) {}
  virtual ~repl_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;
  
  virtual std::string render_coordinates() const {
    std::ostringstream oss;
    oss << "[" << "REPL#" << history_index
        << ", " << line_number << ":"
        << column_number;

    if (value.size() > 1)
      oss << "-" << column_number + value.size() - 1 << "]";
    else
      oss << "]";

    return oss.str();
  }

  virtual repl_token<symbol_type>* copy() const {
    return new repl_token<symbol_type>(*this);
  }
      
 private:
  const int history_index;
  const int line_number;
  const int column_number;
};

template<typename symbol_type>
struct string_token: public token<symbol_type> {
  string_token(symbol_type s, const std::string& v,
               int line_num, int col_num)
      : token<symbol_type>(s, v),
        line_number(line_num),
        column_number(col_num) {}
  virtual ~string_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;

  virtual std::string render_coordinates() const {
    std::ostringstream oss;
    oss << "[" << line_number << ":"
        << column_number;

    if (value.size() > 1)
      oss << "-" << column_number + value.size() - 1 << "]";
    else
      oss << "]";

    return oss.str();
  }

  static std::string render_coordinate_point(int line, int column) {
    std::ostringstream oss;
    oss << "[" << line << ":" << column << "]";
    return oss.str();
  }

  virtual string_token<symbol_type>* copy() const {
    return new string_token<symbol_type>(*this);
  }

 private:
  int line_number;
  int column_number;
};


template<typename symbol_type>
struct file_token: public token<symbol_type> {
  file_token(symbol_type s, const std::string& v,
             const std::string& f, int line_num, int col_num)
      : token<symbol_type>(s, v),
        filename(f), column_number(col_num), line_number(line_num) {}
  virtual ~file_token() {}

  using token<symbol_type>::symbol;
  using token<symbol_type>::value;

  virtual std::string render_coordinates() const {
    std::ostringstream oss;
    oss << "[" << filename << ":"
        << line_number << ":"
        << column_number;

    if (value.size() > 1)
      oss << "-" << column_number + value.size() - 1 << "]";
    else
      oss << "]";

    return oss.str();
  }

  static std::string render_coordinate_point(const std::string& file_name,
                                             int line, int column) {
    std::ostringstream oss;
    oss << "[" << file_name << ":"
        << line << ":"
        << column << "]";
    return oss.str();
  }

  virtual file_token<symbol_type>* copy() const {
    return new file_token<symbol_type>(*this);
  }

 private:
  const std::string filename;
  int column_number;
  int line_number;
};

template<typename symbol_type>
std::ostream& operator<<(std::ostream& stream, const token<symbol_type>& t) {
  stream << t.render_full();
  return stream;
}

#endif /* TOKEN_H */
