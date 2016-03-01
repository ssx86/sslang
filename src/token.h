#ifndef _SSLANG_TOKEN_
#define _SSLANG_TOKEN_

#include <string>

class Token
{
    public:
        typedef enum{
            NEWLINE,       // \n
            STRING,
            ID,
            INT,
            DOUBLE,
			EQ,        // ==
            ADD,       // +
            SUB,       // -
            MUL,       // *
            DIV,       // /
			SELF_ADD,  // ++
			SELF_SUB,  // --
			COMMENT,   // //
			ASSIGN,    // =
            GT,        // >
            GE,        // >=
            LT,        // <
            LE,        // <=
            NE,        // !=
            AND,       // &
            OR,        // |
            BIT_AND,   // &&
            BIT_OR,    // ||
            NOT,       // !
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
			SEMICOLON  // ;
        }Type;

        Token(Type _type);
        Token(int value);
        Token(double value);
        Token(Type _type, const std::string &value); //for ID and STRING

        ~Token();

        Type m_type;

//    private:
    public:
        Token();

		std::string tostring();

        int           ivalue;
        double        dvalue;
        std::string   svalue;

};


#endif
