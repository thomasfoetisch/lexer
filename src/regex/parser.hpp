#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../token.hpp"
#include "../token_buffer.hpp"
#include "alphabet.hpp"
#include "ast/ast.hpp"

namespace regex {

template<typename buffer_type>
class parser {
 public:
  typedef typename buffer_type::token_type token_type;
  typedef typename token_type::symbol_type symbol_type;

  ast::node* parse(buffer_type* buf) {
    return parse_re(buf);
  }

 private:
  ast::node* parse_re(buffer_type* buf) {
    ast::node* result(NULL);
    
    token_type* current(buf->look_ahead(0));
    switch (current->symbol) {
      case symbol::lp: {
        buf->consume();

        ast::node* lhs(parse_re(buf));
        
        if (not buf->match(symbol::rp))
          throw std::string("expected right parenthese.");
        
        result = parse_re_prime(buf, lhs);
        
        break;
      }
      
      case symbol::char_class: {
        buf->consume();

        ast::node* lhs(new ast::char_class(current->value, current));
        result = parse_re_prime(buf, lhs);
        break;
      }

      case symbol::rp: {
        result = new ast::epsilon();
        break;
      }
        
      case symbol::end_of_input:
        break;
        
      default:
        throw std::string("unexpected token: " + current->render());
        break;
    }

    if (not result)
      throw std::string("regex parsing failed for some reason.");
    
    return result;
  }

  ast::node* parse_re_prime(buffer_type* buf, ast::node* lhs) {
    ast::node* result(NULL);
    
    token_type* current(buf->look_ahead(0));
    switch (current->symbol) {
      case symbol::star: {
        if (current->value[0] == '*') {
          buf->consume();

          ast::node* lhs_p(new ast::kleen(lhs, current));
          result = parse_re_prime(buf, lhs_p);
        } else if (current->value[0] == '+') {
          buf->consume();

          ast::node* lhs_p(new ast::cat(lhs->copy(),
                                        new ast::kleen(lhs, current)));
          result = parse_re_prime(buf, lhs_p);
        } else if (current->value[0] == '?') {
          buf->consume();

          ast::node* lhs_p(new ast::alt(lhs, new ast::epsilon()));

          result = parse_re_prime(buf, lhs_p);
        }
        break;
      }

      case symbol::pipe: {
        buf->consume();

        ast::node *lhs_p(new ast::alt(lhs, parse_re(buf), current));
        result = parse_re_prime(buf, lhs_p);
        break;
      }

      case symbol::lp:
      case symbol::char_class: {
        ast::node *lhs_p(new ast::cat(lhs, parse_re(buf)));
        result = parse_re_prime(buf, lhs_p);
        break;
      }

      default:
        result = lhs;
        break;
    }

    if (not result)
      throw std::string("regex parsing failed for some reason.");
    
    return result;
  }
};


}  /* namespace regex */

#endif /* PARSER_H */
