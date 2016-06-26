#include "token.h"
#include <stdio.h>
#include <iostream>

const char* KEYWORDS[] = {"function", "end", "if", "else", "while", "then", "continue", "break", "return", "import", "for"};

Token::Token() {
}

Token::Token(Type _type) 
    : m_type(_type) {
    }
Token::Token(int value) 
    : m_type(INT) {
        ivalue = value;
    }
Token::Token(double value) 
    : m_type(DOUBLE) {
        dvalue = value;
    }
Token::Token(Type _type, const std::string &value) 
    : m_type(_type), svalue(value) {
    }

Token::~Token()
{
}

bool Token::isKeyword() {
    if( m_type != ID )
        return false;

    for(int i = 0; i < sizeof(KEYWORDS) / sizeof(const char*); i++)
    {
        if (svalue == KEYWORDS[i])
            return true;
    }

    return false;
}

std::string Token::tostring() {
    switch(m_type){
        case NEWLINE:
            return "<NEWLINE>";
        case ID:
            if ( isKeyword( ) )
                return std::string("<KEY:") + svalue + ">";
            else
                return std::string("<") + svalue + ">";
        case STRING:
            return std::string("'") + svalue + "'";
        case INT:
            {
                char buff[100];
                sprintf(buff, "%d", ivalue);
                return std::string(buff);
            }
        case DOUBLE:
            {
                char buff[100];
                sprintf(buff, "%lf", dvalue);
                return std::string(buff);
            }
        case DOT:
            return ".";
        case EQ:
            return "==";
        case ASSIGN:
            return "=";
        case GT:
            return ">";
        case LT:
            return "<";
        case GE:
            return ">=";
        case LE:
            return "<=";
        case NE:
            return "~=";
        case NOT:
            return "~";
        case AND:
            return "&&";
        case BIT_AND:
            return "&";
        case OR:
            return "||";
        case BIT_OR:
            return "|";
        case XOR:
            return "^";
        case ADD:
            return "+";
        case SUB:
            return "-";
		case SELF_ADD:
			return "++";
		case SELF_SUB:
			return "--";
        case MUL:
            return "*";
        case DIV:
            return "/";
        case LP:
            return "(";
        case RP:
            return ")";
        case LB:
            return "{";
        case RB:
            return "}";
        case LBRACKET:
            return "[";
        case RBRACKET:
            return "]";
		case COMMA:
			return ",";
		case COLON:
			return ":";
		case SEMICOLON:
			return ";";
		case COMMENT:
			return "//";
		case SHARP:
			return "#";


        default:
            return "<symbol>";
    }
}
