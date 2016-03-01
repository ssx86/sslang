#include "parser.h"
#include "token.h"

// delete
#include <iostream>
#include <sstream>
// delete

Parser::Parser(std::istream* _input) {
    m_input = _input;
    m_lookAhead[0] = 0;
    m_lookAhead[1] = 0;
}

void Parser::skipSpace(){
    char c = lookAhead1();
    while(c == ' ' || c == '\t') {
        next();
        c = lookAhead1();
    }
}

Token* Parser::GetToken() {
    skipSpace();

    char c = lookAhead1();
    if (c == 0)
        return NULL;

    if (isalpha(c)) {
        next();
        std::stringstream ss;
        ss << c;
        while(true) {
            c = lookAhead1();
            if( isalpha(c) ) {
                ss << c;
                next();
            }
            else
                break;
        }
        std::string str;
        ss >> str;
        return new Token(Token::ID, str);
    } else if ( isdigit(c) ) {
        next();
        int n = c - '0';
        while(true) {	
            c = lookAhead1();
            if (isdigit(c)) {
                n = n * 10 + c - '0';
                next();
            }
            else
                break;
        }

        Token* token = new Token(n);
        return token;
    } else {
        next();
        switch (c) {
            case '=': 
                if ('=' == lookAhead1()) {
                    next();
                    return new Token(Token::EQ);
                } else {
                    return new Token(Token::ASSIGN);
                }
            case '+': 
                return new Token(Token::ADD);
            case '-': 
                return new Token(Token::SUB);

            case '/': 
                return new Token(Token::DIV);
            case '*': 
                return new Token(Token::MUL);
            case '\n': 
                return new Token(Token::NEWLINE);
            case '(': 
                return new Token(Token::LP);
            case ')': 
                return new Token(Token::RP);
            case '[': 
                return new Token(Token::LBRACKET);
            case ']': 
                return new Token(Token::RBRACKET);
            case '{': 
                return new Token(Token::LB);
            case '}': 
                return new Token(Token::RB);
            default:
                return new Token(c);
        }
    }

}

bool Parser::Init() {
    // read 2 charactors
    if( !( *m_input >> m_lookAhead[0] >> m_lookAhead[1]) ) {
        std::cerr << "input too short";
        return false;
    }
    return true;
}

bool Parser::next() {
    m_lookAhead[0] = m_lookAhead[1];
    if( !( m_input->get(m_lookAhead[1]) )) {
        m_lookAhead[1] = 0;
        return false;
    }
    return true;
}


char Parser::lookAhead1() {
    return m_lookAhead[0];
}

char Parser::lookAhead2() {
    return m_lookAhead[1];
}
