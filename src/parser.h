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
        ASTNode* chunk() ;
        ASTNode* block() ;
        ASTNode* stat() ;

        ASTNode* goto_stat();
        ASTNode* do_stat();
        ASTNode* while_stat();
        ASTNode* repeat_stat();
        ASTNode* if_stat();
        ASTNode* for_stat();
        ASTNode* range_for_stat();
        ASTNode* function_stat();
        ASTNode* local_function_stat();
        ASTNode* local_namelist_stat();


        ASTNode* retstat() ;
        ASTNode* label() ;
        ASTNode* funcname() ;
        ASTNode* varlist() ;
        ASTNode* var() ;
        bool _var(ASTNode* prefix) ;
        ASTNode* namelist() ;
        ASTNode* explist() ;
        ExpNode* exp() ;
        bool _exp(ExpNode* prefix) ;
        ASTNode* prefixexp() ;
        bool _prefixexp(ASTNode* prefix) ;
        ASTNode* functioncall() ;
        ASTNode* args() ;
        ASTNode* functiondef() ;
        ASTNode* funcbody() ;
        ASTNode* parlist() ;
        ASTNode* tableconstructor() ;
        ASTNode* fieldlist() ;
        ASTNode* field() ;
        bool fieldsep() ;
        ASTNode* binop() ;
        UnopNode* unop() ;
        NameNode* name();
        LeafNode* leaf();
        EmptyNode* empty();
};

#endif
