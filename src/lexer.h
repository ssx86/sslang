#ifndef _SSLANG_LEXER_H_
#define _SSLANG_LEXER_H_

#include <istream>
class Token;

class Lexer
{
    public:
        Lexer(std::istream* _input);
        bool Init();
        Token* GetToken();

    private:
        void next();

        char current();
        char lookAhead1();
        char lookAhead2();

        void skipSpace();
       
    private:
        std::istream* m_input;
        char m_lookAhead[2];
        char m_current;
};

#endif
