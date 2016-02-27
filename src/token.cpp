#include "token.h"

Token::Token()
{
}

Token::Token(Type _type)
    : type(_type)
{}
Token::Token(int value)
    : type(Token::INT)
{}
Token::Token(double value)
    : type(Token::DOUBLE)
{}
Token::Token(Type _type, const std::string &value)
    : type(_type), svalue(value)
{}

Token::~Token()
{}
