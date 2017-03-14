#ifndef ALPHABET_H
#define ALPHABET_H

#include <iostream>


namespace regex {

enum class symbol {
  star, pipe, lp, rp,
  end_of_input,
  char_class
};


std::ostream& operator<<(std::ostream& stream, symbol s);

}  /* namespace regex */

#endif /* ALPHABET_H */
