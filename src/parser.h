#ifndef _SSLANG_PARSER_H_
#define _SSLANG_PARSER_H_

#include <cstddef>
#include <vector>

class Lexer;
class Token;

class ASTNode{
    public:
        Token* op;
        ASTNode* children(int i);
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;
};

class Value {
};

class ASTNodeEvaler{
    public:
        static Value* eval(ASTNode* node){
            return new Value;
        }
};

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
        ASTNode* stat() ;
        ASTNode* retstat() ;
        ASTNode* lable() ;
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
