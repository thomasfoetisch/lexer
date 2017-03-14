#ifndef MATCH_H
#define MATCH_H

#include <string>
#include "dfa.hpp"

namespace regex {


bool match(const dfa& automaton, const std::string& str);


}  /* namespace regex */

#endif /* MATCH_H */
