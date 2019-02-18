# What is lexer
Lexer is a small C++ library which implement a regex based lexer
engine. There is very often situation where one need to implement a
simple language (read: implement a lexer, a parser and possibly a
abstract syntax tree). If the language is simple enough, it is easy to
write for example a recursive descent parser, and we can often
restrict to simpler grammar than GLR.

However, one always needs to implement a lexer, and the more token it
can recognize, the trickier it becomes, with subtle corner cases. It
can be a frustrating experience, especially when we just want to
experiment with the implementation of a new language.

This lexer library solves this problem. It makes easy to build a fully
functional lexer based on regex expression for each recognized
token. The regexes are compiled into a DFA at run-time which makes token
recognition fast. The longer token is always recognized, and if two
or more token regexes conflicts, one can define a priority, and the
token with the highest priority will be recognized.

Contrary with flex for example, lexer build the definitions of the
tokens and compile the NFA to a DFA at run-time, and everything is
done in C++. This can be an issue where high performance is required,
but makes it easy to experiment and debug.

# How to use lexer
To use lexer, you should first fork/clone the repository:

    git clone https://github.com/thomashilke/lexer.git


then change directory and build the project:

    cd lexer && make

You can then include the header in lexer/include/ and link your
project with the static library lib/liblexer.a:

    g++ myproject.cpp -o myproject -I /path/to/lexer/include -L /path/to/lexer/lib -llexer


# Minimal usage example
The minimal example consists in defining a bunch of symbols, whose
lexemes set are defined by regular expressions, and using thoses to
build a regex-based lexer. The lexer can then be used to tokenize a
sequence of characters.

We first include the main lexer header:

    #include <lexer/lexer.hpp>


For the sake of the example, we will tokenize comma-separated lists of
(integer) numbers, double-quoted strings, and true/false boolean
values. That is, the symbol set of this simple language is {comma,
number, string, boolean}, and is represented as an enum class:

    enum class symbol {
      comma, number, boolean, string, eoi
    };
    
    using token_type = token<symbol>;

Here we omit some helper function. These can be found in the complete
source file `examples/list_lexer.cpp`. The lexemes are defined in a
function `build_list_lexer`:

    regex_lexer<token_type> build_list_lexer() {
      regex_lexer_builder<token_type> rlb(symbol::eoi);
      
      rlb.emit(symbol::comma, ",");
      rlb.emit(symbol::number, "-?[1-9][0-9]*");
      rlb.emit(symbol::string, "\"[^\"]*\"");
      rlb.emit(symbol::boolean, "(true)|(false)");
  
      rlb.skip("[ \r\n\t]*");
      
      return rlb.build();
    }


And finally the main function. We initialize a regex-lexer and a file
character sequence source. We loop until the end-of-input symbol is
reached, and print each recognized token:

    int main(int argc, char *argv[]) {
      if (argc != 2)
        return 0;
      
      try {
        const std::string filename(argv[1]);
        std::ifstream f(filename.c_str(), std::ios::in);
      
        regex_lexer<token_type> l(build_list_lexer());
    
        file_source<token_type> fs(&f, filename);
        l.set_source(&fs);
    
        token_type* t(l.get());
        while (t->symbol != symbol::eoi) {
          std::cout << t->symbol << "(" << t->value << ") at "
                    << t->render_coordinates() << std::endl;
          delete t; t = l.get();
        }
        delete t; t = nullptr;
      }
      catch (const std::string& e) {
        std::cout << e << std::endl;
      }
      
      return 0;
    }

The complete source file can be found in `examples/list_lexer.cpp` and
compiled with:

    $ cd examples; g++ -o list_lexer list_lexer.cpp -I ../include ../lib/liblexer.a
    
Given the content of the file `list.txt`

    true, false, 1234, "hello"

we can run the example:
    
    $ ./list_lexer list.txt
    boolean(true) at [list.txt:0:0-3]
    comma(,) at [list.txt:0:4]
    boolean(false) at [list.txt:0:6-10]
    comma(,) at [list.txt:0:11]
    number(1234) at [list.txt:0:13-16]
    comma(,) at [list.txt:0:17]
    string("hello") at [list.txt:0:19-25]
