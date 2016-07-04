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
            PERCENT,   // %
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


//    private:
    public:
        Token();

		std::string tostring();

        int           ivalue;
        double        dvalue;
        std::string   svalue;


};


#endif
