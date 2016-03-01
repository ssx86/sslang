#ifndef _SSLANG_PARSER_H_
#define _SSLANG_PARSER_H_

class ASTNode{
};
class Lexer;

class Parser
{
    public:
        Parser(Lexer* lexer);
        ASTNode* Parse();

        bool next();

    private:
        ASTNode* lookAhead1();
        ASTNode* lookAhead2();

    private:
        Lexer* m_pLexer;
        ASTNode* m_lookAhead[2];
};

#endif
