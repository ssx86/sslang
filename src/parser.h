#ifndef _SSLANG_PARSER_H_
#define _SSLANG_PARSER_H_

#include <cstddef>
#include <vector>
#include <string>
#include <stack>

#include "node.h"

class Lexer;
class Token;


class Parser
{
    public:
        Parser(Lexer* lexer);
        ASTNode* Parse();

        bool match(std::string pattern, int offset = 0);
        bool match(Token::Type type, int offset = 0);
        void next(Token::Type type = Token::UNKNOWN);
        void next(const char* str);
        void do_next();

        void debug_print(std::string str);
        void enter(std::string str);
        void leave();
        int level;

        std::stack<std::string> m_stack;

    private:
        Token* getTokenFromOffset(int offset);
        Token* current();
        Token* lookAhead1();
        Token* lookAhead2();

    private:
        ASTNode* m_root;
        Lexer* m_pLexer;
        Token* m_current;
        Token* m_lookAhead[2];

    private:
        BlockNode* chunk() ;
        BlockNode* block() ;
        ASTNode* stat() ;

        GotoNode* goto_stat();
        DoNode* do_stat();
        WhileNode* while_stat();
        RepeatNode* repeat_stat();
        IfNode* if_stat();
        ForNode* for_stat();
        ForNode* range_for_stat();
        FunctionNode* function_stat();
        ASTNode* local_function_stat();
        ASTNode* local_namelist_stat();


        RetNode* retstat() ;
        LabelNode* label() ;
        FuncNameNode* funcname() ;
        VarListNode* varlist() ;
        ASTNode* var() ;
        ASTNode* _var(ASTNode* prefix) ;
        NameListNode* namelist() ;
        ExpListNode* explist() ;
        ASTNode* exp() ;
        ASTNode* _exp(ASTNode* prefix) ;
        ASTNode* prefixexp() ;
        ASTNode* _prefixexp(ASTNode* prefix) ;
        ASTNode* functioncall() ;
        ASTNode* args() ;
        ASTNode* functiondef() ;
        FunctionBodyNode* funcbody() ;
        ASTNode* parlist() ;
        ASTNode* tableconstructor() ;
        FieldListNode* fieldlist() ;
        FieldNode* field() ;
        bool fieldsep() ;
        Token* binop() ;
        UnopNode* unop() ;
        NameNode* name();
        LeafNode* leaf();
        EmptyNode* empty();
};

#endif
