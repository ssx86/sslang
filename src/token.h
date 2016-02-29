#ifndef _SSLANG_TOKEN_
#define _SSLANG_TOKEN_

#include <string>

class Token
{
    public:
        typedef enum{
            STRING,
            ID,
            INT,
            DOUBLE,
            ADD,           // +
            SUB,           // -
            MUL,           // *
            DIV,           // /
            AND,           // &
            OR,            // |
            BIT_AND,       // &&
            BIT_OR,        // ||
            NOT,           // !
            INVERSION,     // ~
            PERCENT,       // %
            BACKSLASH,      
            L_PAREN,       // (
            R_PAREN,       // )
            L_BRACE,       // {
            R_BRACE,       // }
            L_BRACKET,     // [
            R_BRACKET,     // ]
        }Type;

        Token(Type _type);
        Token(int value);
        Token(double value);
        Token(Type _type, const std::string &value); //for ID and STRING

        ~Token();

        Type type;

//    private:
    public:
        Token();

        int           ivalue;
        double        dvalue;
        std::string   svalue;
        char          c;

};


#endif
