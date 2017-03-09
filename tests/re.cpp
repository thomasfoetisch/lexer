
#include <string>

#include "../regex/lexer_string.hpp"
#include "../token_stream.hpp"
#include "../regex/ast/ast.hpp"
#include "../regex/dfa.hpp"
#include "../regex/build_dfa.hpp"
#include "../regex/match.hpp"
#include "../regex/parser.hpp"

int main(int argc, char *argv[]) {
  if (not (argc == 3))
    return 0;
  
  try {
    regex::lexer_string lex(argv[1]);
    token_stream<regex::lexer_string> tok_stream(&lex);

    regex::parser p;

    regex::ast::node* root(new regex::ast::cat(p.parse_re<regex::lexer_string>(&tok_stream),
                                               new regex::ast::end_marker()));

    regex::dfa_context c;
    root->initialize_positions(1);
    root->build_last_pos();
    root->build_first_pos();

    root->populate(c);

    root->print();

    c.print();
    
    regex::dfa automaton(regex::build_dfa(c, root));

    std::cout << "does '" << argv[2] << "' matches /" << argv[1] << "/? "
              << regex::match(automaton, argv[2]) << std::endl;
  }
  catch(const std::string& e) {
    std::cout << e << std::endl;
  }

  
  return 0;
}
