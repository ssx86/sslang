#ifndef _SSLANG_LEXER_H_
#define _SSLANG_LEXER_H_

#include <istream>
#include <map>

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
       
        bool is_keyword(std::string&);

    private:
        std::istream* m_input;
        char m_lookAhead[2];
        char m_current;

        std::map<std::string, int> m_keyword_map;
};

#endif
