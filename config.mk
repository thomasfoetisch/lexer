CXX = g++
DEPS_BIN = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g

BIN_DIR = ~/.local/bin/

SOURCES = test/file_source_test.cpp \
          test/lexer_test.cpp \
          test/regex_lexer_test.cpp \
          test/regex_parser_test.cpp \
          test/string_source_test.cpp \
          test/test_regex_ast.cpp \
          test/token_buffer_test.cpp \
          test/token_test.cpp

HEADERS = include/lexer/lexer.hpp \
	  include/lexer/token_buffer.hpp \
	  include/lexer/token.hpp \
	  include/lexer/regex/alphabet.hpp \
	  include/lexer/regex/build_dfa.hpp \
	  include/lexer/regex/dfa.hpp \
	  include/lexer/regex/lexer.hpp \
	  include/lexer/regex/match.hpp \
	  include/lexer/regex/parser.hpp \
	  include/lexer/regex/ast/alt.hpp \
	  include/lexer/regex/ast/ast.hpp \
	  include/lexer/regex/ast/cat.hpp \
	  include/lexer/regex/ast/char_class.hpp \
	  include/lexer/regex/ast/end_marker.hpp \
	  include/lexer/regex/ast/epsilon.hpp \
	  include/lexer/regex/ast/kleen.hpp \
	  include/lexer/regex/ast/marker.hpp \
	  include/lexer/regex/ast/node.hpp

BIN = bin/file_source_test \
      bin/lexer_test \
      bin/regex_lexer_test \
      bin/regex_parser_test \
      bin/string_source_test \
      bin/token_buffer_test \
      bin/token_test

bin/file_source_test: build/test/file_source_test.o
bin/lexer_test: build/test/lexer_test.o
bin/regex_lexer_test: build/test/regex_lexer_test.o
bin/regex_parser_test: build/test/regex_parser_test.o
bin/string_source_test: build/test/string_source_test.o
bin/token_buffer_test: build/test/token_buffer_test.o
bin/token_test: build/test/token_test.o
