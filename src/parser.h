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
        
        enum{
            OPENED,
            CLOSED
        } m_input_status;
       
    private:
        std::istream* m_input;
        char m_ahead[2];
};

#endif
