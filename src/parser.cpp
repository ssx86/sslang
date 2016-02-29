#include "parser.h"
#include "token.h"

// delete
#include <iostream>
#include <sstream>
// delete

Parser::Parser(std::istream* _input)
{
	m_input = _input;
	m_lookAhead[0] = 0;
	m_lookAhead[1] = 0;
}


Token* Parser::GetToken()
{
	char c = lookAhead1();
	if (c == 0)
		return NULL;

	if (isalpha(c))
	{
		std::stringstream ss;
		ss << c;
		next();
		while(true)
		{
			c = lookAhead1();
			if( isalpha(c) )
			{
				ss << c;
				next();
			}
			else
				break;
		}
		std::string str;
		ss >> str;
		return new Token(Token::ID, str);
	}
	else if ( isdigit(c) ) 
	{
		int n = c - '0';
		next();
		while(true)
		{	
			c = lookAhead1();
			if (isdigit(c))
			{
				n = n * 10 + c - '0';
				next();
			}
			else
				break;
		}

		Token* token = new Token(n);
		return token;
	}
	else 
	{
		Token* token = NULL;

		switch (c)
		{
		case '=':
			{
				token = new Token(Token::EQ);
				break;
			}
		case '+':
			{
				token = new Token(Token::ADD);
				break;
			}
		case '-':
			{
				token = new Token(Token::SUB);
				break;
			}
		case '/':
			{
				token = new Token(Token::DIV);
				break;
			}
		case '*':
			{
				token = new Token(Token::MUL);
				break;
			}
		default:
			token = new Token(c);
		}
		next();
		return token;
	}

}

bool Parser::Init()
{
	// read 2 charactors
	if( !( *m_input >> m_lookAhead[0] >> m_lookAhead[1]) )
	{
		std::cerr << "input too short";
		return false;
	}
	return true;
}

bool Parser::next()
{
	m_lookAhead[0] = m_lookAhead[1];
	if( !((*m_input) >> m_lookAhead[1]) )
	{
		m_lookAhead[1] = 0;
		return false;
	}
	return true;
}


char Parser::lookAhead1()
{
	return m_lookAhead[0];
}

char Parser::lookAhead2()
{
	return m_lookAhead[1];
}
