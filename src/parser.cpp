#include "parser.h"
#include "token.h"

// delete
#include <iostream>
// delete

Parser::Parser(std::istream* _input)
{
    m_input = _input;
	m_lookAhead[0] = 0;
	m_lookAhead[1] = 0;
}


Token* Parser::GetToken()
{
	if (lookAhead1() == 0)
        return NULL;

    Token* token = new Token(lookAhead1());

	next();
    return token;
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
