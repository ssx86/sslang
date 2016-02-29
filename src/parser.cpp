#include "parser.h"
#include "token.h"

// delete
#include <iostream>
// delete

Parser::Parser(std::istream* _input)
{
    m_input = _input;
}


Token* Parser::GetToken()
{
    if (!next())
        return NULL;
    Token* token = new Token(lookAhead2());
    return token;
}

bool Parser::Init()
{
    // read 2 charactors
    if( !(next() && next()) )
    {
         std::cerr << "input too short";
         return false;
    }
    m_input_status = OPENED;
    return true;
}

bool Parser::next()
{
    if( !((*m_input) >> m_ahead[1]) )
    {
        m_input_status = CLOSED;
        return false;
    }
    return true;
}

char Parser::lookAhead1()
{
    return m_ahead[0];
}

char Parser::lookAhead2()
{
    return m_ahead[1];
}
