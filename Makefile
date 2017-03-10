CXX = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g

BIN_DIR = ~/.local/bin/

BIN_TEST = bin/file_source_test \
           bin/lexer_test \
           bin/regex_lexer_test \
           bin/regex_parser_test \
           bin/string_source_test \
           bin/token_buffer_test \
           bin/token_test

OBJECTS = build/test/file_source_test.o \
          build/test/lexer_test.o \
          build/test/regex_lexer_test.o \
          build/test/regex_parser_test.o \
          build/test/string_source_test.o \
          build/test/test_regex_ast.o \
          build/test/test_regex.o \
          build/test/token_buffer_test.o \
          build/test/token_test.o

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


.PHONY = all clean install


$(HEADERS): include/%: src/%
	@echo [INSTALL] $(<:src/%=%)
	@install -m 0644 -D $< $@

all: $(BIN_TEST) $(HEADERS)

$(OBJECTS): build/%.o: %.cpp
	@echo [CXX] $<
	@mkdir --parents $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_TEST): bin/%: build/test/%.o
	@echo [LD] $@
	@$(CXX) $(LDFLAGS) -o $@ $<

clean:
	@rm -f $(OBJECTS)
	@rm -f $(BIN) $(BIN_TEST)
	@rm -rf include/*

install: bin/...
	cp bin/... $(BIN_DIR)
