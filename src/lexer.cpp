#include "lexer.h"
#include "token.h"

// delete
#include <iostream>
#include <sstream>
// delete

Lexer::Lexer(std::istream* _input) {
    m_input = _input;
    m_lookAhead[0] = 0;
    m_lookAhead[1] = 0;
}

void Lexer::skipSpace(){
    char c = lookAhead1();
    while(c == ' ' || c == '\t') {
        next();
        c = lookAhead1();
    }
}

Token* Lexer::GetToken() {
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
        bool match_point = false; // matched "."
        std::stringstream ss;
        while(true) {	
            c = lookAhead1();
            if (match_point && c == '.') { // 1.2.3
                break;
            }
            
            if (isdigit(c) || c == '.') {
                if ( c == '.' ) {
                    match_point = true;
                }
                ss << c;
                next();
            }
            else
                break;
        }
        if(match_point) {
            double n;
            ss >> n;
            return new Token(n);
        } else {
            int n;
            ss >> n;
            return new Token(n);
        }

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

bool Lexer::Init() {
    // read 2 charactors
    if( !( *m_input >> m_lookAhead[0] >> m_lookAhead[1]) ) {
        std::cerr << "input too short";
        return false;
    }
    return true;
}

bool Lexer::next() {
    m_lookAhead[0] = m_lookAhead[1];
    if( !( m_input->get(m_lookAhead[1]) )) {
        m_lookAhead[1] = 0;
        return false;
    }
    return true;
}


char Lexer::lookAhead1() {
    return m_lookAhead[0];
}

char Lexer::lookAhead2() {
    return m_lookAhead[1];
}
