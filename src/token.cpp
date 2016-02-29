#include "token.h"

Token::Token()
{
}

Token::Token(Type _type)
	: m_type(_type)
{}
Token::Token(int value)
	: m_type(INT)
{
	ivalue = value;
}
Token::Token(double value)
	: m_type(DOUBLE)
{
	dvalue = value;
}
Token::Token(Type _type, const std::string &value)
	: m_type(_type), svalue(value)
{}

Token::~Token()
{}

std::string Token::tostring()
{
	switch(m_type){
	case ID:
		{
			return std::string("<id, ") + svalue + ">";
		}
	case STRING:
		{
			return std::string("<string, ") + svalue + ">";
		}
	case INT:
		{
			char buff[100];
			sprintf(buff, "<int, %d>", ivalue);
			return std::string(buff);
		}
	case DOUBLE:
		{
			char buff[100];
			sprintf(buff, "<double, %lf>", dvalue);
			return std::string(buff);
		}
	case EQ:
		return "<=>";
	case ADD:
		return "<+>";
	case SUB:
		return "<->";
	case MUL:
		return "<*>";
	case DIV:
		return "</>";

	default:
		{
			return "<symbol>";
		}
	}
}