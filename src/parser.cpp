#include "parser.h"
#include "lexer.h"
#include "token.h"

#include <cstdlib>
#include <iostream>

void Parser::debug_print(std::string str) {
    for(int i = 0; i < level; i++)
        std::cout << "  ";
    std::cout << "[" << current()->tostring() << "] " << str << std::endl;;
}

void Parser::enter(std::string str) {
    level ++;
    for(int i = 0; i < level; i++)
        std::cout << "  ";
    std::cout << "[ENTER]: " <<  str << std::endl;;
}

void Parser::leave() {
    for(int i = 0; i < level; i++)
        std::cout << "  ";
    std::cout << "[LEAVE]: "  << current()->tostring() << std::endl;;
    level --;
}

void ASTNode::addChild(ASTNode* child) {
    m_children.push_back(child);
}

ASTNode* ASTNode::children(int i) {
    return m_children[i];
}

Parser::Parser(Lexer* lexer) 
: m_root(NULL), level(-1) {
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

void Parser::next(Token::Type type) {

    //预判
    if ( type != Token::UNKNOWN && m_lookAhead[0] 
            && m_lookAhead[0]->isType(type) ) {
        std::cout << "[" << current()->tostring() << "] " << "error!, expected << " << type << std::endl;
        exit(1);
    }
    do {
        m_current = m_lookAhead[0];
        m_lookAhead[0] = m_lookAhead[1];
        m_lookAhead[1] = m_pLexer->GetToken();   
    } while (m_current->isType(Token::NEWLINE) );
    for(int i = 0; i < level; i++)
        std::cout << "  ";
    std::cout << "{" << current()->tostring() << "}" << std::endl;

}

Token* Parser::getTokenFromOffset(int offset) {
    Token* token = NULL;
    switch(offset) {
        case 0:
            token = current(); break;
        case 1:
            token = lookAhead1(); break;
        case 2:
            token = lookAhead2(); break;
    }
    return token;
}



ASTNode* Parser::Parse() {
    m_root = new ASTNode();
    while( ASTNode* block = chunk() ) {
        m_root->addChild(block);
    }
    return m_root;
}

bool Parser::match(std::string pattern, int offset) {
    Token *token = getTokenFromOffset(offset);

    //string(id/keyword)
    if (token->isType(Token::ID) && token->svalue == pattern)
        return true;
    else
        return false;
}

bool Parser::match(Token::Type type, int offset) {
    Token *token = getTokenFromOffset(offset);

    if (token->isType(type))
        return true;
    else
        return false;
}

/*
 * chunk ::= block
 */
ASTNode* Parser::chunk() {
    ASTNode* node =  block();
    return node;
}

/*
 * block ::= {stat} [retstat]
 */
ASTNode* Parser::block() {

    enter("block");
    ASTNode* node = new ASTNode;
    if (!match("return")) {
        debug_print("going to match stat");
        while( ASTNode* stat_node = stat() ) {
            node->addChild(stat_node);
        }
        debug_print("going to match stat end");
    }
    if (match("return")) /* TODO: retstat */
    {
        debug_print("return, matching");
        ASTNode* retstat_node = retstat();
        node->addChild(retstat_node);
        debug_print("return, matching end");
    }
    leave();
    return node;
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
  
    std::cout << "-------------------------------------" << std::endl;
    enter("stat");

    if(match(Token::SEMICOLON) )
    {
        next();
        leave();
        return new ASTNode;
    } else if( match ("function") ) {
        next();
        ASTNode* node = new ASTNode;
        //funcname
        debug_print(" --> funcname() ");
        node->addChild(funcname());
        debug_print(" --> funcbody() ");
        node->addChild(funcbody());
        debug_print(" --> funcbody() end");
        leave();
        return node;
    } else if( match ("if") ) {
        debug_print(" try to match if ");
        ASTNode* ifNode = new ASTNode;
        next();

        ASTNode* pathNode = new ASTNode;

        ASTNode* condNode = exp();
        debug_print(" eat then ");
        next(); // then
        ASTNode* blockNode = block();
        pathNode->addChild(condNode);
        pathNode->addChild(blockNode);

        ifNode->addChild(pathNode);

        while(match("elseif")) {
            next();
            ASTNode* pathNode = new ASTNode;

            ASTNode* condNode = exp();
            next(); // then
            ASTNode* blockNode = block();
            pathNode->addChild(condNode);
            pathNode->addChild(blockNode);

            ifNode->addChild(pathNode);
        }
        if(match("else")) {
            debug_print(" try match else ");
            next();
            ASTNode* pathNode = new ASTNode;
            ASTNode* condNode = NULL;
            ASTNode* blockNode = block();
            pathNode->addChild(condNode);
            pathNode->addChild(blockNode);

            ifNode->addChild(pathNode);
        }
        debug_print(" eat end ");
        next(); // "end"
        debug_print("got an if node");

        leave();
        return ifNode;

    } else if(match("break")
            || match ("local")
            || match ("for")
            || match ("repeat")
            || match ("do")
            || match ("goto")
            || match ("while")
            || match (Token::SEMICOLON)
            //   || match ("::")
      ) {
        next();

        leave();
        return NULL;
    } else if(match("end") || match("return")) {

        leave();
        return NULL; // 特殊处理
    }

    debug_print("matching varlist");


    bool is_varlist_not_functioncall = false;
    ASTNode* varlist_or_functioncall_Node = new ASTNode;
    if( match(Token::ID) || match(Token::LP) ) {
        debug_print("fuck start!!!");
        ASTNode* varNode = var();
        debug_print("fuck end!!!");
        varlist_or_functioncall_Node->addChild(varNode);
        debug_print("got a varnode");
        if(match(Token::COLON) || match(Token::LP)) {
            debug_print("it is a function call!");
            is_varlist_not_functioncall = false; // functioncall!
            if(match(Token::COLON) ) {
                varlist_or_functioncall_Node->addChild(new ASTNode(current()));
                next();
                varlist_or_functioncall_Node->addChild(new ASTNode(current()));
                next();
            }
            ASTNode* argsNode = args();
            varlist_or_functioncall_Node->addChild(argsNode);
            debug_print("got a function call!");

            leave();
            return varlist_or_functioncall_Node;
        }else{

            while (match(Token::COMMA)) {
                is_varlist_not_functioncall = true;
                next(Token::COMMA);  // eat ,
                ASTNode* varNode = var();
                varlist_or_functioncall_Node->addChild(varNode);
                debug_print("add something into var");
            }

            next(Token::ASSIGN); 
            ASTNode* explistNode = explist();

            debug_print("got a varlist = explist");
            leave();
            return varlist_or_functioncall_Node;
        }
    }

    debug_print("error!!!! no match");
}

/*
 * retstat ::= return [explist] [‘;’]
 */
ASTNode* Parser::retstat() {
    enter("retstat");

    next(); // eat key word 'return'

    debug_print("try explist");
    ASTNode* explistNode = explist();
    debug_print("try explist end");
    ASTNode* retStatNode = new ASTNode;
    retStatNode->addChild(explistNode);

    debug_print(" got a retstat ");
    leave();
    return retStatNode;
}


/*
 * label ::= ‘::’ Name ‘::’
*/
ASTNode* Parser::label() {
    enter("lable");
    leave();
    return NULL;
}

/*
 * funcname ::= Name {‘.’ Name} [‘:’ Name]
 */
ASTNode* Parser::funcname() {
    enter("funcname");
    ASTNode* node = new ASTNode();
    node->addChild(new ASTNode(current()));
    next();

    if(match(Token::DOT) || match(Token::COLON) ) {
        node->addChild(new ASTNode(current()));
        next();
        node->addChild(new ASTNode(current()));
        next();
    }

    leave();
    return node;
}

/*
 * varlist ::= var {‘,’ var}
 */
ASTNode* Parser::varlist() {
    enter("varlist");
    leave();
    return NULL;
}

/*
 * // var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name 
 *
 * var ::= Name `var | ( exp ) `prefixexp [ exp ] `var | ( exp ) `prefixexp . Name `var
 */
ASTNode* Parser::var() {
    enter("var");

    ASTNode* node = new ASTNode;
    if(match(Token::ID) ) {
        node->addChild(new ASTNode(current()));
        next();
        _var(node);
    }else if(match(Token::LP)) {
        next(Token::LP);
        ASTNode* expNode = exp();
        node->addChild(expNode);

        _prefixexp(node);

        if(match(Token::LBRACKET)) {
            next(Token::LBRACKET);
            ASTNode* expNode2 = exp();
            node->addChild(expNode2);
            next(Token::RBRACKET);

            _var(node);
        }
        else if (match(Token::DOT)) {
            next(Token::DOT);
            //next is Name
            node->addChild(new ASTNode(current()));
            next();
            _var(node);
        }
    }

    leave();
    return node;
}

/*
 * `var ::= `prefixexp [ exp ] `var | `prefixexp . Name `var | nil
*/
bool Parser::_var(ASTNode* prefix) {
    enter("_var");
    _prefixexp(prefix);

    //lookforward : [ .
    if (match(Token::LBRACKET)) {
        next(Token::LBRACKET);
        ASTNode* expNode = exp();
        prefix->addChild(expNode);
        next(Token::RBRACKET);
    }
    else if(match(Token::DOT)) {
        prefix->addChild(new ASTNode(current()));
        next(Token::DOT);
        prefix->addChild(new ASTNode(current()));
        next(Token::ID);
    }
    else
    {
        leave();
        return false;
    }

    _var(prefix);

    leave();
    return true;
}

/*
 * namelist ::= Name {‘,’ Name}
 */
ASTNode* Parser::namelist() {
    enter("namelist");

    ASTNode* namelistNode = new ASTNode;

    ASTNode* nameNode = new ASTNode(current());
    next();

    namelistNode->addChild(nameNode);

    while(match(Token::COMMA)) {
        next();

        ASTNode* nameNode = new ASTNode(current());
        next();

        namelistNode->addChild(nameNode);
    }

    leave();
    return namelistNode;
}

/*
 * explist ::= exp {‘,’ exp}
 */
ASTNode* Parser::explist() {
    enter("explist");

    ASTNode* explistNode = new ASTNode;
    debug_print("try exp");
    ASTNode* expNode = exp();
    debug_print("try exp end");
    explistNode->addChild(expNode);
    while(match(Token::COMMA)){
        next(Token::COMMA);
        ASTNode* expNode = exp();
        explistNode->addChild(expNode);
    }
    leave();
    debug_print("got an explist");
    return explistNode;

}

/*
 * //  exp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef |
 * // prefixexp | tableconstructor | exp binop exp | unop exp 
 *
 * exp ::= (nil | false  | true  | Numeral  |
 *         LiteralString  | '...'  | functiondef   |
 *         prefixexp  | tableconstructor  | unop exp )  `exp
 */
ASTNode* Parser::exp() {
    enter("exp");
    ASTNode* expNode = new ASTNode;
    if (match ("nil") ||
            match ("false") ||
            match ("true") ||
            match (Token::STRING) ||
            match (Token::INT) ||
            match (Token::DOUBLE) ||
            match ("...")  // TODO
       )
    {  
        debug_print(" this is an int ");
        ASTNode* node = new ASTNode(current());
        next();
        expNode->addChild(node);
    } else {
        // functiondef, prefixexp, tableconstructor, unop
        if (match(Token::LB)) {
            //table
            ASTNode* tableNode = tableconstructor();
            expNode->addChild(tableNode);
        } else if (match("function")) {
            //func
            ASTNode* functiondefNode = functiondef();
            expNode->addChild(functiondefNode);
        } 
        // unop
        else {
            ASTNode* testNode = unop();
            if(testNode) { // is unop
                expNode->addChild( testNode );
            } else {
                testNode = prefixexp();
                expNode->addChild(testNode);
                debug_print("got a prefixexp exp exp !!!");
            }
        }
    }
    _exp(expNode);

    debug_print("got a expNode");
    leave();
    return expNode;
}

/* 
 * `exp ::= binop exp | nil
 */
bool Parser::_exp(ASTNode* prefix) {
    enter("_exp");
    ASTNode* testNode = binop();
    if (testNode) {
        prefix->addChild(testNode);
        ASTNode* expNode = exp();
        prefix->addChild(expNode);
        leave();
        return true;
    } else {
        leave();
        return false;
    }
}

/*
 * // prefixexp ::= var | functioncall | ‘(’ exp ‘)’
 * prefixexp ::= var `prefixexp  | ( exp ) `prefixexp
 */
ASTNode* Parser::prefixexp() {
    enter("prefixexp");
    ASTNode* node = NULL;
    if( ! match(Token::LP) ) {
        node = var();
    } else {
        next(Token::LP);
        node = exp();
        next(Token::RP);
    }
    _prefixexp(node);

    leave();
    return node;
}

/*
 * `prefixexp ::= args `prefixexp  | ':' Name args `prefixexp | ""
 */
bool Parser::_prefixexp(ASTNode* prefix) {
    enter("_prefixexp");
    if ( match (Token::COLON) ) {
        prefix->addChild(new ASTNode(current()));
        next();
        prefix->addChild(new ASTNode(current()));
        next();
        ASTNode* argsNode = args();
        prefix->addChild(argsNode);
        _prefixexp(prefix);
        leave();

        return true;
    } else {
        ASTNode* argsNode = args();
        if (argsNode) {
            prefix->addChild(argsNode);
            _prefixexp(prefix);
            leave();
            return true;
        }
        else
        {
            leave();
            return false;
        }
    }

}


/*
 * functioncall ::=  prefixexp args | prefixexp ‘:’ Name args 
 */
ASTNode* Parser::functioncall() {
    enter("functioncall");
    ASTNode* node = new ASTNode;
    ASTNode* prefixexpNode = prefixexp();
    node->addChild(prefixexpNode);
    if(match(Token::COLON)) {
        node->addChild(new ASTNode(current()));
        next(Token::COLON);
        node->addChild(new ASTNode(current()));
        next(Token::ID);
    }
    ASTNode* argsNode = args();
    node->addChild(argsNode);

    leave();
    return node;
}

/*
 * args ::=  ‘(’ [explist] ‘)’ | tableconstructor | LiteralString 
 */
ASTNode* Parser::args() {
    enter("args");
    // args的lookforwards
    if( !match(Token::LP) && !match(Token::LB) && !match(Token::STRING) ) {
        leave();
        return NULL;
    }
    if( match(Token::LP) ) {
        next();
        if(!match(Token::RP)) {
            ASTNode* explistNode = explist();
            next(Token::RP);
            leave();
            return explistNode;
        }
    } else if ( match(Token::STRING) ) {
        ASTNode *argsNode = new ASTNode(current());
        next();
        leave();
        return argsNode;
    } else if ( match(Token::LB) ) {
        ASTNode *argsNode = tableconstructor();
        leave();
        return argsNode;
    }
    leave();
    return NULL;
}

/*
 * functiondef ::= function funcbody
 */
ASTNode* Parser::functiondef() {
    enter("functiondef");
    next(); 

    ASTNode* funcbodyNode = funcbody();

    leave();
    return funcbodyNode;
}

/*
 * funcbody ::= ‘(’ [parlist] ‘)’ block end
 */
ASTNode* Parser::funcbody() {
    enter("funcbody");
    ASTNode* node = new ASTNode;
    next(Token::LP);
    ASTNode* parlistNode = parlist();
    next(Token::RP);

    debug_print("------------------------");
    ASTNode* blockNode = block();
    debug_print("------------------------");

    debug_print("End");
    next(Token::ID); // "end"
    node->addChild(parlistNode);
    node->addChild(blockNode);
    leave();
    return node ;
}

/*
 * parlist ::= namelist [‘,’ ‘...’] | ‘...’
 */
ASTNode* Parser::parlist() {
    enter("parlist");
    ASTNode* namelistNode = NULL;
    if(match(Token::RP)) {
        debug_print("got a null parlist");
        leave();
        return NULL;
    }else if(match("...")) {
        ASTNode* node =  new ASTNode(current());
        next();
        namelistNode->addChild(node);
        debug_print("got a ... parlist");
        leave();
        return namelistNode;
    } else {
        namelistNode = namelist();
        if(match(Token::COMMA)) {
            namelistNode->addChild(new ASTNode(current()));
            next();
        }
        debug_print("got a parlist");
        leave();
        return namelistNode;
    }

}

/*
 * tableconstructor ::= ‘{’ [fieldlist] ‘}’
 */
ASTNode* Parser::tableconstructor() {
    enter("tableconstructor");
    next(Token::LB);
    ASTNode* fieldlistNode = fieldlist();
    next(Token::RB);

    leave();
    return fieldlistNode;
}

/*
 * fieldlist ::= field {fieldsep field} [fieldsep]
 */
ASTNode* Parser::fieldlist() {
    enter("fieldlist");
    ASTNode* node = new ASTNode;
    
    ASTNode* fieldNode = field();
    node->addChild(fieldNode);
    while(fieldsep()) {
        if ( !match(Token::RB) ) {
            ASTNode* fieldNode = field();
            node->addChild(fieldNode);
        }
    }
    debug_print("got a list!" );
    leave();
    return node;
}

/*
 * field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
 */
ASTNode* Parser::field() {
    enter("field");
    ASTNode* fieldNode = NULL;
    if (match(Token::LBRACKET) ) {
        next();
        fieldNode = exp();
        next();
        if(match(Token::ASSIGN)) {
            ASTNode* expNode = exp();
            fieldNode->addChild(expNode);
        }
    } else if (match(Token::ID) && match(Token::ASSIGN, 1)) {
        fieldNode = new ASTNode(current());
        ASTNode* expNode = exp();
        fieldNode->addChild(expNode);
    } else {
        fieldNode = exp();
    }
    leave();
    return NULL;
}

/*
 * fieldsep ::= ‘,’ | ‘;’
 */
ASTNode* Parser::fieldsep() {
    enter("fieldsep");
    if(match(Token::SEMICOLON) || match (Token::COMMA)) {
        ASTNode* node = new ASTNode(current());
        next();
        leave();
        return node;
    }
    leave();
    return NULL;
}

/*
 * binop ::=  ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | 
 * ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’ | 
 * ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | 
 * and | or
 */
ASTNode* Parser::binop() {
    enter("binop");
    if(match(Token::ADD) ||
            match(Token::SUB) ||
            match(Token::MUL) ||
            match(Token::DIV) ||
            match(Token::COMMENT) ||
            match(Token::XOR) ||
            match(Token::PERCENT) ||
            match(Token::BIT_AND) ||
            match(Token::BIT_OR) ||
            match(Token::AND) ||
            match(Token::OR) ||
            match(Token::NOT) ||
            //match(Token::MOVER) ||
            //match(Token::MOVEL) ||
            //match(Token::RANGE) ||
            match(Token::LT) ||
            match(Token::LE) ||
            match(Token::GT) ||
            match(Token::GE) ||
            match(Token::EQ) ||
            match(Token::NE) ||
            match("or") ||
            match("and")
      ) 
    {
        ASTNode* binopNode = new ASTNode(current());
        next();
        leave();
        return binopNode;
    } else {
        leave();
        return NULL;
    }
}

/*
 * unop ::= ‘-’ | not | ‘#’ | ‘~’
 */
ASTNode* Parser::unop() {
    enter("unop");
    if(match(Token::SUB) ||
            match(Token::SHARP) ||
            match(Token::NOT) ||
            match("not")
      )
    {
        ASTNode* unopNode = new ASTNode(current());
        next();
        leave();
        return unopNode;
    }
    leave();
    return NULL;
}
