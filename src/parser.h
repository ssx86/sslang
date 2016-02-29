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

        char LookAhead1();
        char LookAhead2();
       
    private:
        std::istream* input;
        char ahead[2];
};

#endif
