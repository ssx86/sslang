#ifndef _SSLANG_PARSER_H_
#define _SSLANG_PARSER_H_

#include <istream>
class Token;

class Parser
{
    public:
        Parser(std::istream* _input);
        bool Init();
        Token* GetToken();

    private:
        bool next();

        char lookAhead1();
        char lookAhead2();

        void skipSpace();
       
    private:
        std::istream* m_input;
        char m_lookAhead[2];
};

#endif
