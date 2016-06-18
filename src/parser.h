#ifndef _SSLANG_PARSER_H_
#define _SSLANG_PARSER_H_

#include <cstddef>
#include <vector>

#include "node.h"

class Lexer;
class Token;


class Parser
{
    public:
        Parser(Lexer* lexer);
        ASTNode* Parse();

        void next();

    private:
        Token* current();
        Token* lookAhead1();
        Token* lookAhead2();

    private:
        ASTNode* m_root;
        Lexer* m_pLexer;
        Token* m_current;
        Token* m_lookAhead[2];

    private:
        ASTNode* chunk() ;
        ASTNode* block() ;
        StatNode* stat() ;
        RetStatNode* retstat() ;
        ASTNode* label() ;
        ASTNode* funcname() ;
        ASTNode* varlist() ;
        ASTNode* var() ;
        ASTNode* namelist() ;
        ASTNode* explist() ;
        ASTNode* exp() ;
        ASTNode* prefixexp() ;
        ASTNode* functioncall() ;
        ASTNode* args() ;
        ASTNode* functiondef() ;
        ASTNode* funcbody() ;
        ASTNode* parlist() ;
        ASTNode* tableconstructor() ;
        ASTNode* fieldlist() ;
        ASTNode* field() ;
        ASTNode* fieldsep() ;
        ASTNode* binop() ;
        ASTNode* unop() ;
};

#endif
