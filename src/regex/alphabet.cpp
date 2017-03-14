
#include "alphabet.hpp"


namespace regex {
  
std::ostream& operator<<(std::ostream& stream, symbol s) {
  switch (s) {
    case symbol::end_of_input:
      stream << "eoi";
      break;
    case symbol::star:
      stream << "kleen";
      break;
    case symbol::pipe:
      stream << "alt";
      break;
    case symbol::lp:
      stream << "lp";
      break;
    case symbol::rp:
      stream << "rp";
      break;
    case symbol::char_class:
      stream << "class";
      break;
    default:
      break;
  }

  return stream;
}

}
