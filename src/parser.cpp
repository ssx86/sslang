#include "parser.h"
#include "lexer.h"
#include "token.h"

#include <iostream>

/*
class ASTNode{
    public:
        Token* op;
        ASTNode* children(int i);
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;
};
*/

void ASTNode::addChild(ASTNode* child) {
    m_children.push_back(child);
}

ASTNode* ASTNode::children(int i) {
    return m_children[i];
}

Parser::Parser(Lexer* lexer) 
: m_root(NULL) {
    m_pLexer = lexer;
    m_current =lexer->GetToken(); 
    m_lookAhead[0] = lexer->GetToken();
    m_lookAhead[1] = lexer->GetToken();

}

Token* Parser::lookAhead1() {
    return m_lookAhead[0];
}

Token* Parser::lookAhead2() {
    return m_lookAhead[1];
}

Token* Parser::current() {
    return m_current;
}

void Parser::next() {
    m_current = m_lookAhead[0];
    m_lookAhead[0] = m_lookAhead[1];
    m_lookAhead[1] = m_pLexer->GetToken();   
}

ASTNode* Parser::Parse() {
    m_root = new ASTNode();
    while( ASTNode* block = chunk() ) {
        m_root->addChild(block);
    }
    return m_root;
}

/*
 * chunk ::= block
 */
ASTNode* Parser::chunk() {
    return block();
}

/*
 * block ::= {stat} [retstat]
 */
ASTNode* Parser::block() {
    stat();
    if (false /*TODO: restart */) {
        retstat();
    }
    return NULL;
}

/*
 * stat ::=  ‘;’ | 
 *      varlist ‘=’ explist | 
 *      functioncall | 
 *      label | 
 *      break | 
 *      goto Name | 
 *      do block end | 
 *      while exp do block end | 
 *      repeat block until exp | 
 *      if exp then block {elseif exp then block} [else block] end | 
 *      for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end | 
 *      for namelist in explist do block end | 
 *      function funcname funcbody | 
 *      local function Name funcbody | 
 *      local namelist [‘=’ explist] 
 */
ASTNode* Parser::stat() {
    return NULL;
}

/*
 * retstat ::= return [explist] [‘;’]
 */
ASTNode* Parser::retstat() {
    return NULL;
}


/*
 * label ::= ‘::’ Name ‘::’
*/
ASTNode* Parser::lable() {
    return NULL;
}

/*
 * funcname ::= Name {‘.’ Name} [‘:’ Name]
 */
ASTNode* Parser::funcname() {
    return NULL;
}

/*
 * varlist ::= var {‘,’ var}
 */
ASTNode* Parser::varlist() {
    return NULL;
}

/*
 * var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name 
 */
ASTNode* Parser::var() {
    return NULL;
}

/*
 * namelist ::= Name {‘,’ Name}
 */
ASTNode* Parser::namelist() {
    return NULL;
}

/*
 * explist ::= exp {‘,’ exp}
 */
ASTNode* Parser::explist() {
    return NULL;
}

/*
 * exp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef |
 * prefixexp | tableconstructor | exp binop exp | unop exp 
 */
ASTNode* Parser::exp() {
    return NULL;
}

/*
 * prefixexp ::= var | functioncall | ‘(’ exp ‘)’
 */
ASTNode* Parser::prefixexp() {
    return NULL;
}

/*
 * functioncall ::=  prefixexp args | prefixexp ‘:’ Name args 
 */
ASTNode* Parser::functioncall() {
    return NULL;
}

/*
 * args ::=  ‘(’ [explist] ‘)’ | tableconstructor | LiteralString 
 */
ASTNode* Parser::args() {
    return NULL;
}

/*
 * functiondef ::= function funcbody
 */
ASTNode* Parser::functiondef() {
    return NULL;
}

/*
 * funcbody ::= ‘(’ [parlist] ‘)’ block end
 */
ASTNode* Parser::funcbody() {
    return NULL;
}

/*
 * parlist ::= namelist [‘,’ ‘...’] | ‘...’
 */
ASTNode* Parser::parlist() {
    return NULL;
}

/*
 * tableconstructor ::= ‘{’ [fieldlist] ‘}’
 */
ASTNode* Parser::tableconstructor() {
    return NULL;
}

/*
 * fieldlist ::= field {fieldsep field} [fieldsep]
 */
ASTNode* Parser::fieldlist() {
    return NULL;
}

/*
 * field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
 */
ASTNode* Parser::field() {
    return NULL;
}

/*
 * fieldsep ::= ‘,’ | ‘;’
 */
ASTNode* Parser::fieldsep() {
    return NULL;
}

/*
 * binop ::=  ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | 
 * ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’ | 
 * ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | 
 * and | or
 */
ASTNode* Parser::binop() {
    return NULL;
}

/*
 * unop ::= ‘-’ | not | ‘#’ | ‘~’
 */
ASTNode* Parser::unop() {
    return NULL;
}
