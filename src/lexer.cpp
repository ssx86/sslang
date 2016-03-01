#include "lexer.h"
#include "token.h"

// delete
#include <iostream>
// delete

#include <sstream>

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

    char c = current();
    if (c == 0)
        return NULL;

    if (isalpha(c)) {
        std::stringstream ss;
        ss << c;
        char next_c;
        while(true) {
            ss << c;
            next_c = lookAhead1();
            if( isalpha(next_c) || isdigit(next_c) ) {
                next();
                c = current();
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
            ss << c;

            char next_c = lookAhead1();
            if (match_point && next_c == '.') { // 1.2.3
                break;
            }
            
            if (isdigit(next_c) || next_c == '.') {
                if ( c == '.' ) {
                    match_point = true;
                }
                next();
                c = current();
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
                if ('=' == current() ) {
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
    if (!m_input->get(m_current) 
            && m_input->get(m_lookAhead[0])
            && m_input->get(m_lookAhead[1])) {
        std::cerr << "input too short";
        return false;
    }else{
        return true;
    }
}

void Lexer::next() {
    m_current = m_lookAhead[0];
    m_lookAhead[0] = m_lookAhead[1];
    if( !( m_input->get(m_lookAhead[1]) )) {
        m_lookAhead[1] = 0;
    }
}

char Lexer::current(){
    return m_current;
}

char Lexer::lookAhead1() {
    return m_lookAhead[0];
}

char Lexer::lookAhead2() {
    return m_lookAhead[1];
}
