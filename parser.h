#ifndef _SSLANG_PARSER_H_
#ifndef _SSLANG_PARSER_H_

class Token;

class Parser
{
    public:
        Token* Next();
    private:
        char LookAhead1();
        char LookAhead2();
    private:
        char ahead[2];
}

#endif
