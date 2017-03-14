#ifndef BUILD_DFA_H
#define BUILD_DFA_H


#include "dfa.hpp"
#include "ast/ast.hpp"


namespace regex {


dfa build_dfa(dfa_context& c, ast::node* root);


}  /* namespace regex */

#endif /* BUILD_DFA_H */
