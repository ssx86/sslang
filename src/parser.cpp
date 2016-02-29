#include "parser.h"
#include "token.h"

// delete
#include <iostream>
// delete

Parser::Parser(std::istream* _input)
{
    input = _input;
}


Token* Parser::GetToken()
{
    if (!next())
        return NULL;
    Token* token = new Token(ahead[1]);
    return token;
}

bool Parser::Init()
{
    if(!next())
    {
         std::cerr << "input too short";
         return false;
    }
    return true;
}

bool Parser::next()
{
    if( !((*input) >> ahead[1]) )
            return false;
    return true;
}
