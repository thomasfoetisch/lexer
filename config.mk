CXX = clang++
DEPS_BIN = g++
CXXFLAGS = -O2 -std=c++11 -fPIC
LDFLAGS = -O2 -fPIC
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p


PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/

PKG_NAME = lexer

SOURCES = test/file_source.cpp \
          test/lexer.cpp \
          test/regex_lexer.cpp \
          test/regex_parser.cpp \
          test/string_source.cpp \
          test/regex_ast.cpp \
          test/token_buffer.cpp \
          test/token.cpp \
          src/regex/symbols.cpp \
          src/regex/build_dfa.cpp \
          src/regex/match.cpp \
	  src/minimal_lexer.cpp \
	  test/circuit_graph.cpp

HEADERS = include/lexer/lexer.hpp \
	  include/lexer/regex_lexer.hpp \
	  include/lexer/regex_lexer_builder.hpp \
	  include/lexer/source_coordinate_range.hpp \
	  include/lexer/token_buffer.hpp \
	  include/lexer/token.hpp \
	  include/lexer/regex/symbols.hpp \
	  include/lexer/regex/build_dfa.hpp \
	  include/lexer/regex/dfa.hpp \
	  include/lexer/regex/lexer.hpp \
	  include/lexer/regex/match.hpp \
	  include/lexer/regex/source.hpp \
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

BIN = bin/test_file_source \
      bin/test_lexer \
      bin/test_regex_lexer \
      bin/test_regex_parser \
      bin/test_string_source \
      bin/test_token_buffer \
      bin/test_token \
      bin/minimal_lexer \
      bin/circuit_graph


bin/test_file_source: build/test/file_source.o build/src/regex/symbols.o
bin/test_lexer: build/test/lexer.o build/src/regex/symbols.o build/src/regex/build_dfa.o
bin/test_regex_lexer: build/test/regex_lexer.o build/src/regex/symbols.o
bin/test_regex_parser: build/test/regex_parser.o build/src/regex/symbols.o
bin/test_string_source: build/test/string_source.o build/src/regex/symbols.o
bin/test_token_buffer: build/test/token_buffer.o build/src/regex/symbols.o
bin/test_token: build/test/token.o build/src/regex/symbols.o
bin/circuit_graph: build/test/circuit_graph.o build/src/regex/symbols.o build/src/regex/build_dfa.o
bin/minimal_lexer: build/src/minimal_lexer.o build/src/regex/symbols.o build/src/regex/build_dfa.o

LIB = lib/liblexer.a

lib/liblexer.a: build/src/regex/symbols.o build/src/regex/build_dfa.o build/src/regex/match.o
