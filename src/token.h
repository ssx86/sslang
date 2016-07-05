#ifndef _SSLANG_TOKEN_
#define _SSLANG_TOKEN_

#include <string>

class Token
{
    public:
        typedef enum{
            NEWLINE = 100,       // \n
            STRING,
            ID,
            KEYWORD,
            INT,
            DOUBLE,
            DOT,       // .
            DOTDOT,    // ..
			EQ,        // ==
            ADD,       // +
            SUB,       // -
            MUL,       // *
            DIV,       // /
			COMMENT,   // --
			ASSIGN,    // =
            GT,        // >
            GE,        // >=
            LT,        // <
            LE,        // <=
            NE,        // ~=
            BIT_AND,   // &
            BIT_OR,    // |
            XOR,       // ^
            AND,       // &&
            OR,        // ||
            NOT,       // ~
            MOD,       // %
            BACKSLASH, // 
            LP,        // (
            RP,        // )
            LB,        // {
            RB,        // }
            LBRACKET,  // [
            RBRACKET,  // ]
			COMMA,     // ,
			COLON,     // :
			LABEL,     // ::
			SEMICOLON, // ;
            SHARP,     // #
            UNKNOWN

        }Type;

        Token(Type _type);
        Token(int value);
        Token(double value);
        Token(Type _type, const std::string &value); //for ID and STRING


        ~Token();

        Type m_type;

        Type type() {
            return m_type;
        }

        bool isType(Type t) {
            return m_type == t;
        }
        bool isKey(std::string key) {
            if (( type() == ID) && key == svalue) {
                return true;
            } else {
                return false;
            }
        }
        int pri () { // priority
            switch (m_type) {
                case LT:
                case LE:
                case GT:
                case GE:
                case NE:
                case EQ:
                    return 7;
                case BIT_OR:
                    return 8;
                case NOT:
                    return 9;
                case BIT_AND:
                    return 10 ;
                //case MOVEL:
                //case MOVER:
                  //return 11;
                    case DOTDOT:
                        return 12;
                case ADD:
                case SUB:
                   return 13;
                case MUL:
                case DIV:
                case MOD:
                   // case "//"
                   return 14;
                case ID:
                   if (svalue == "or")
                       return 5;
                   else if (svalue == "and")
                       return 6;
                   else if (svalue == "not")
                       return 15;
                default:
                   return 0;
            }
        }



//    private:
    public:
        Token();

		std::string tostring();

        int           ivalue;
        double        dvalue;
        std::string   svalue;


};


#endif
