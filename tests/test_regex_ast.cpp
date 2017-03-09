#include "regex_ast.hpp"

int main(int argc, char *argv[]) {
  using regex::ast::node;
  using regex::ast::cat;
  using regex::ast::kleen;
  using regex::ast::alt;
  using regex::ast::char_class;
  using regex::ast::end_marker;
  
  node* regex(new cat(
      new cat(
          new cat(
              new kleen(
                  new alt(
                      new char_class("a"),
                      new char_class("b"))),
              new char_class("a")),
          new char_class("b")),
      new char_class("b")));

  node* root(new cat(regex, new end_marker()));

  regex::dfa_context c;

  root->initialize_positions(1);
  root->build_last_pos();
  root->build_first_pos();

  root->populate(c);
  
  root->print();

  c.print();
  
  regex::regex_dfa dfa(build_dfa(c, root));

  std::cout << "does 'aababaabbaababb' matches '(a|b)*abb'? " << match(dfa, "aababaabbaababb") << std::endl;
  std::cout << "does 'aababaabbaababbbb' matches '(a|b)*abb'? " << match(dfa, "aababaabbaababbbb") << std::endl;

  
  return 0;
}
