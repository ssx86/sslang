#include "lexer.h"
#include "token.h"

// delete
#include <iostream>
// delete

#include <sstream>

const char* KEYWORDS[] = {"function", "end", "if", "else", "while", "then", "continue", "break", "return", "import", "for"};

Lexer::Lexer(std::istream* _input) {
	m_input = _input;
	m_lookAhead[0] = 0;
	m_lookAhead[1] = 0;
}

void Lexer::skipSpace(){
	char c = current();
	while(c == ' ' || c == '\t') {
		next();
		c = current();
	}
}

int Lexer::getLineNo() {
    return m_lineno;
}

Token* Lexer::GetToken() {
	skipSpace();

	char c = current();
	if (c == 0)
		return NULL;

	if (isalpha(c) || '_' == c) {
		std::stringstream ss;
		while(true) {
			ss << c;
			next();
			c = current();
			if( !(isalpha(c) || isdigit(c) || '_' == c)) {
				break;
			}
		}
		return new Token(Token::ID, ss.str());
	} else if ('"' == c || '\'' == c) {
		char ending = c;
		next();
		char c = current();
		std::stringstream ss;
		while( (c = current()) != ending ) {	
			ss << c;
			next();
		}
		next(); // eat the ending quote

		return new Token(Token::STRING, ss.str());
	} else if ( isdigit(c) ) {
		bool match_point = false; // matched "."
		std::stringstream ss;
		while(true) {	
			ss << c;
			next();
			c = current();
			if (match_point && c == '.') { // 1.2.3
				break;
			}

			if (isdigit(c) || c == '.') {
				if ( c == '.' ) {
					match_point = true;
				}
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
        case '.':
            if('.' == current()) {
                next();
                return new Token(Token::DOTDOT);
            } else{
                return new Token(Token::DOT);
            }
		case '=': 
			if ('=' == current() ) {
				next();
				return new Token(Token::EQ);
			} else {
				return new Token(Token::ASSIGN);
			}
		case '>':
			if ('=' == current()) {
				next();
				return new Token(Token::GE);
			} else {
				return new Token(Token::GT);
			}
		case '<':
			if ('=' == current()) {
				next();
				return new Token(Token::LE);
			} else {
				return new Token(Token::LT);
			}
		case '+': 
            return new Token(Token::ADD);
		case '-': 
			if ('-' == current() ) {
                if( m_lookAhead[0] == '[' && m_lookAhead[1] == '[') {
                    do{ 
                        next();
                        if(current() == '\n')
                            m_lineno++;
                    } while ( ']' != m_lookAhead[0] || ']' != m_lookAhead[1]);
                    next(); next(); next();
                } else {
                    do{ next();} while ( '\n' != current() );
                }
				return new Token(Token::COMMENT);
			} else {
				return new Token(Token::SUB);
			}
		case '/': 
			if ('/' == current() ) {
				next();
				while(current() != '\n')
					next();
				return new Token(Token::COMMENT);
			} else {
				return new Token(Token::DIV);
			}
        case '|':
            if ('|' == current() ) {
                next();
                return new Token(Token::OR);
            } else {
                return new Token(Token::BIT_OR);
            }
        case '&':
            if ('&' == current() ) {
                next();
                return new Token(Token::AND);
            } else {
                return new Token(Token::BIT_AND);
            }
        case '^':
            return new Token(Token::XOR);
		case '*': 
			return new Token(Token::MUL);
		case '\n': 
            m_lineno++;
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
		case ',':
			return new Token(Token::COMMA);
		case ':':
            if (':' == current() )
                return new Token(Token::LABEL);
            else
                return new Token(Token::COLON);
		case ';':
			return new Token(Token::SEMICOLON);
		case '~':
            if ('=' == current() ) {
                next();
                return new Token(Token::NE);
            } else {
                std::cerr << std::endl << "Unknown charactor: " << c << std::endl;
                return NULL;
            }
		case '#':
			return new Token(Token::SHARP);
		default:
			std::cerr << std::endl << "Unknown charactor: " << c << std::endl;
			return new Token(c);
		}
	}

}

bool Lexer::Init() {
	if (!(m_input->get(m_current) 
		&& m_input->get(m_lookAhead[0])
		&& m_input->get(m_lookAhead[1]))) {
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

bool Lexer::isKeyword(std::string& str) {

    for(int i = 0; i < sizeof(KEYWORDS) / sizeof(const char*); i++)
    {
        if (str == KEYWORDS[i])
            return true;
    }

    return false;
}

