#ifndef SOURCE_COORDINATE_RANGE_H
#define SOURCE_COORDINATE_RANGE_H

#include <string>
#include <iostream>
#include <sstream>


struct source_coordinate_range {
public:
  source_coordinate_range(std::size_t line,
                          std::size_t column,
                          std::size_t range_size)
    : line(line), column(column), range_size(range_size) {}
  virtual ~source_coordinate_range() {}

  std::size_t get_line() const { return line; }
  std::size_t get_column() const { return column; }
  std::size_t get_range_size() const { return range_size; }

  virtual std::string render() const {
    std::ostringstream oss;

    oss << '[' << line << ':' << column;
    if (range_size > 1)
      oss << '-' << column + range_size - 1 << ']';
    else
      oss << ']';

    return oss.str();
  }

  virtual source_coordinate_range* copy() const {
    return new source_coordinate_range(*this);
  }

protected:
  const std::size_t line;
  const std::size_t column;
  const std::size_t range_size;
};


struct file_source_coordinate_range: public source_coordinate_range {
public:
  file_source_coordinate_range(const std::string& filename,
                               std::size_t line,
                               std::size_t column,
                               std::size_t range_size)
    : source_coordinate_range(line, column, range_size),
      filename(filename) {}
  virtual ~file_source_coordinate_range() {}

  virtual std::string render() const {
    std::ostringstream oss;

    oss << '[' << filename << ':' << line << ':' << column;
    if (range_size > 1)
      oss << '-' << column + range_size - 1 << ']';
    else
      oss << ']';

    return oss.str();
  }

  virtual source_coordinate_range* copy() const {
    return new file_source_coordinate_range(*this);
  }

  const std::string& get_filename() const {
    return filename;
  }

protected:
  const std::string filename;
};


struct repl_source_coordinate_range: public source_coordinate_range {
public:
  repl_source_coordinate_range(std::size_t hist_idx,
                               std::size_t line,
                               std::size_t column,
                               std::size_t range_size)
    : source_coordinate_range(line, column, range_size),
      history_index(hist_idx) {}
  virtual ~repl_source_coordinate_range() {}

  virtual std::string render() const {
    std::ostringstream oss;
    oss << "[" << "REPL#" << history_index
        << ", " << line << ":"
        << column;

    if (range_size > 1)
      oss << "-" << column + range_size - 1 << "]";
    else
      oss << "]";

    return oss.str();
  }

  virtual source_coordinate_range* copy() const {
    return new repl_source_coordinate_range(*this);
  }

protected:
  const std::size_t history_index;
};

#endif /* SOURCE_COORDINATE_RANGE_H */
