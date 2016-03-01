#include "parser.h"
#include "lexer.h"
#include "token.h"

Parser::Parser(Lexer* lexer) {
    m_pLexer = lexer;
    m_lookAhead[0] = NULL;
    m_lookAhead[1] = NULL;
}

bool Parser::next() {
	return false;
}



