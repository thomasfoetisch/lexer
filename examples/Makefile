all: list_lexer

list_lexer: list_lexer.cpp
	@g++ -o $@ $< -I ../include/ ../lib/liblexer.a


clean:
	@rm -rf list_lexer
	@rm -rf *~
