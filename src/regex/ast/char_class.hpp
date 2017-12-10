#ifndef CHAR_CLASS_H
#define CHAR_CLASS_H

#include <iostream>
#include <set>
#include <string>
#include "../dfa.hpp"
#include "node.hpp"

namespace regex {
namespace ast {


class char_class: public node {
 public:
  using node::token_type;
  
  explicit char_class(const std::string& c, token_type* t)
      : representation(256, false), coordinates(t->render()) {
    if (not c.size())
      throw std::string("char_class(string, token_type): "
                        "empty class not allowed.");

    if (c.size() == 1) {
      if (c[0] == '.') {
        set_range(0, 255);
      } else {
        representation[c[0]] = true;
      }
    } else if (c[0] == '\\') {
      process_escaped_char(c[1]);
    } else if (c.front() == '[' and c.back() == ']') {
      process_character_class(c);
    } else {
      throw std::string("char_class(string, token_type*): "
                        "unrecognized class description.");
    }
  }

  void process_character_class(const std::string& c) {
    std::string::const_iterator it(c.begin() + 1);
    std::string::const_iterator end(c.end() - 1);

    bool is_negated(false);
    if (*it == '^') {
      is_negated = true;
      ++it;
    }

    if (*it == '-') {
      representation['-'] = true;
      ++it;
    }

    while (it != end) {
      if (*it == '\\') {
        if (it + 3 != end and *(it + 2) == '-') {
          throw std::string("process_character_class(string): "
                            "escaped character are not allowed "
                            "in range description.");
        } else {
          process_escaped_char(*(it+1));
          it += 2;
        }
      } else if (*(it + 1) == '-' and it + 2 != end) {
        set_range(*it, *(it + 2));
        it += 3;
      } else {
        representation[*it] = true;
        it += 1;
      }
    }

    if (is_negated)
      for (std::vector<bool>::iterator i(representation.begin());
           i != representation.end();
           ++i)
        *i = not *i;
  }

  void process_escaped_char(unsigned char c) {
      switch (c) {
        // standard ascii escape sequences
        case '0': representation[0] = true; break;
        case 'a': representation[7] = true; break;
        case 'b': representation[8] = true; break;
        case 't': representation[9] = true; break;
        case 'n': representation[10] = true; break;
        case 'v': representation[11] = true; break;
        case 'f': representation[12] = true; break;
        case 'r': representation[13] = true; break;
        case 'e': representation[27] = true; break;


          //  regex special character escape sequences
        case '[': representation['['] = true; break;
        case ']': representation[']'] = true; break;
        case '(': representation['('] = true; break;
        case ')': representation[')'] = true; break;
        case '|': representation['|'] = true; break;
        case '*': representation['*'] = true; break;
        case '+': representation['+'] = true; break;
        case '?': representation['?'] = true; break;
        case '.': representation['.'] = true; break;
        case '\\': representation['\\'] = true; break;

          // lower case 
        case 'l':
          set_range('a', 'z');
          break;
          
          //upper case
        case 'u':
          set_range('A', 'Z');
          break;
          
          // digits
        case 'd':
          set_range('0', '9');
          break;
          
          // space
        case 's':
          representation[' '] = true;
          representation['\t'] = true;
          representation['\r'] = true;
          representation['\n'] = true;
          representation['\f'] = true;
          break;
          
          // word
        case 'w':
          set_range('a', 'z');
          set_range('A', 'Z');
          set_range('0', '9');
          representation['_'] = true;
          break;

        default:
          throw std::string("process_escaped_char(char): "
                            "unknown escaped character.");
      }
          
  }
  
  void set_range(unsigned char start, unsigned char end) {
    for (int i(start); i < end + 1; ++i)
      representation[i] = true;
  }

  bool match(unsigned char c) { return representation[c]; }

  virtual int initialize_positions(int start) {
    position = start;
    return start + 1;
  }

  int get_position() const { return position; }
    
  virtual bool nullable() {
    return false;
  }
  
  virtual void build_first_pos() {
    node::first_pos.insert(position);
  }
  
  virtual void build_last_pos() {
    node::last_pos.insert(position);
  }

  virtual void populate(dfa_context& c) {
    c.positions[position] = this;
  }

  virtual node* copy() {
    return new char_class(*this);
  }
  
  void print() {
    std::cout << "char_class: " << coordinates;
    for (std::vector<bool>::iterator it(representation.begin());
         it != representation.end();
         ++it)
      std::cout << *it << " ";
    std::cout << std::endl;
    
      
    node::print();
  }

 protected:
  int position;
  std::vector<bool> representation;

  const std::string coordinates;
};


}  /* namespace ast */
}  /* namespace regex */

#endif /* CHAR_CLASS_H */
