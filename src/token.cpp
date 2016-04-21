#include "token.h"
#include <stdio.h>
#include <iostream>

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

std::string Token::tostring() {
    switch(m_type){
        case NEWLINE:
            return "\n";
        case ID:
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
            return "!=";
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


        default:
            return "<symbol>";
    }
}
