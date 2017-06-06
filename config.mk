CXX = g++
DEPS_BIN = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p


PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/


SOURCES = test/file_source_test.cpp \
          test/lexer_test.cpp \
          test/regex_lexer_test.cpp \
          test/regex_parser_test.cpp \
          test/string_source_test.cpp \
          test/test_regex_ast.cpp \
          test/token_buffer_test.cpp \
          test/token_test.cpp \
          src/regex/alphabet.cpp \
          src/regex/build_dfa.cpp \
          src/regex/match.cpp \
	  test/circuit_graph.cpp

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
      bin/token_test \
      bin/circuit_graph


bin/file_source_test: build/test/file_source_test.o build/src/regex/alphabet.o
bin/lexer_test: build/test/lexer_test.o build/src/regex/alphabet.o build/src/regex/build_dfa.o
bin/regex_lexer_test: build/test/regex_lexer_test.o build/src/regex/alphabet.o
bin/regex_parser_test: build/test/regex_parser_test.o build/src/regex/alphabet.o
bin/string_source_test: build/test/string_source_test.o build/src/regex/alphabet.o
bin/token_buffer_test: build/test/token_buffer_test.o build/src/regex/alphabet.o
bin/token_test: build/test/token_test.o build/src/regex/alphabet.o
bin/circuit_graph: build/test/circuit_graph.o build/src/regex/alphabet.o build/src/regex/build_dfa.o

LIB = lib/liblexer.a

lib/liblexer.a: build/src/regex/alphabet.o build/src/regex/build_dfa.o build/src/regex/match.o
