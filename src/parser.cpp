#include "parser.h"
#include "lexer.h"
#include "token.h"

#include <cstdlib>
#include <iostream>

#define DEBUG_ENTER 0

void Parser::debug_print(std::string str) {
    std::cout << m_pLexer->getLineNo() << ":\t";
#if DEBUG_ENTER
    //for(int i = 0; i < level; i++)
    //std::cout << "  ";
#else
    std::cout <<  str << std::endl;;
#endif

}

void Parser::enter(std::string str) {
    (void)str;
#if DEBUG_ENTER
    std::cout << m_pLexer->getLineNo() << ":\t";
    level ++;
    //for(int i = 0; i < level; i++)
    //std::cout << "  ";
    std::cout << "[ENTER]: " <<  str << std::endl;;
    m_stack.push(str);
#endif
}

void Parser::leave() {
#if DEBUG_ENTER
    std::string str = m_stack.top();
    m_stack.pop();

    std::cout << m_pLexer->getLineNo() << ":\t";
    //for(int i = 0; i < level; i++)
    //std::cout << "  ";
    std::cout << "[LEAVE]: "  << str << std::endl;;
    level --;
#endif

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

void Parser::do_next() {
    do {
        m_current = m_lookAhead[0];
        m_lookAhead[0] = m_lookAhead[1];
        m_lookAhead[1] = m_pLexer->GetToken();   

        if (!current()) {
            std::cout << "end of file" << std::endl;
            break;
        }
    } while (m_current->isType(Token::NEWLINE) );

    if(current()) {
#if DEBUG_ENTER
        std::cout << "Token: {" << current()->tostring() << "}" << std::endl;
#endif
    }
}


void Parser::next(const char* str) {
    //预判
    if (  current() && (!current()->isType(Token::ID) || current()->svalue != str )) {
        std::cout << m_pLexer->getLineNo() << "[" << current()->tostring() << "] " << "error!, expected : " << str << ", got : " << current()->svalue << std::endl;
        exit(1);
    }
    do_next();
}
void Parser::next(Token::Type type) {

    //预判
    if ( type != Token::UNKNOWN && current()
            && !current()->isType(type) ) {
        std::cout << m_pLexer->getLineNo() << "[" << current()->tostring() << "] " << "error!, expected : " << Token(type).tostring() << ", got : " << current()->tostring() << std::endl;
        exit(1);
    }
    do_next();

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

    if(!token)
        return false;

    //string(id/keyword)
    if (token->isType(Token::ID) && token->svalue == pattern)
        return true;
    else
        return false;
}

bool Parser::match(Token::Type type, int offset) {
    Token *token = getTokenFromOffset(offset);

    if(!token)
        return false;

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
    if (current() == NULL) 
        return NULL;
    enter("block");

    ASTNode* node = new ASTNode;
    if (!match("return")) {
        while( ASTNode* stat_node = stat() ) {
            node->addChild(stat_node);
        }
    }
    if (match("return")) /* TODO: retstat */
    {
        ASTNode* retstat_node = retstat();
        node->addChild(retstat_node);
    }
    leave();
    return node;
}


/* goto Name */
ASTNode* Parser::goto_stat() {
    next("goto");
    ASTNode* gotoNode = new ASTNode;
    NameNode* nameNode = name();

    gotoNode->addChild(nameNode);
    return gotoNode;
}

/*      // do block end  */
ASTNode* Parser::do_stat() {
    enter("do");
    ASTNode* doNode = new ASTNode;
    next("do");

    ASTNode* blockNode = block();
    next("end");

    doNode->addChild(blockNode);

    leave();
    return blockNode;
}

/* while exp do block end | */
ASTNode* Parser::while_stat() {
    next("while"); // while
    ASTNode* whileNode = new ASTNode;
    ASTNode* expNode = exp();

    next("do"); // do

    ASTNode* blockNode = block();

    next("end"); // end

    whileNode->addChild(expNode);
    whileNode->addChild(blockNode);

    return whileNode;
}

/* repeat block until exp */
ASTNode* Parser::repeat_stat() {
    enter("repeat");

    next("repeat");
    ASTNode* repeatNode = new ASTNode;
    ASTNode* blockNode = block();
    next("until");
    ASTNode* expNode = exp();
    repeatNode->addChild(blockNode);
    repeatNode->addChild(expNode);

    leave();
    return repeatNode;
}

ASTNode* Parser::if_stat() {
    enter("if");

    ASTNode* ifNode = new ASTNode;
    next("if");

    ASTNode* pathNode = new ASTNode;

    ASTNode* condNode = exp();
    next("then"); // then
    ASTNode* blockNode = block();
    pathNode->addChild(condNode);
    pathNode->addChild(blockNode);

    ifNode->addChild(pathNode);

    while(match("elseif")) {
        next("elseif");
        ASTNode* pathNode = new ASTNode;

        ASTNode* condNode = exp();
        next("then"); // then
        ASTNode* blockNode = block();
        pathNode->addChild(condNode);
        pathNode->addChild(blockNode);

        ifNode->addChild(pathNode);
    }
    if(match("else")) {
        next("else");
        ASTNode* pathNode = new ASTNode;
        ASTNode* condNode = NULL;
        ASTNode* blockNode = block();
        pathNode->addChild(condNode);
        pathNode->addChild(blockNode);

        ifNode->addChild(pathNode);
    }

    next("end"); // "end"

    leave();
    return ifNode;
}

/* for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end |  */
ASTNode* Parser::for_stat() {
    enter("for_stat");
    next("for"); // for

    ForNode* forNode = new ForNode;

    NameNode* nameNode = name();
    forNode->setName(nameNode);

    next(Token::ASSIGN); // =

    ASTNode* fromExpNode = exp();
    forNode->setFrom(fromExpNode);

    next(Token::COMMA);

    ASTNode* toExpNode = exp();
    forNode->setTo(toExpNode);

    if(match(Token::COMMA) ) {
        next(Token::COMMA);
        ASTNode* stepExpNode = exp();
        forNode->setStep(stepExpNode);
    }
    next("do"); // do

    ASTNode* blockNode = block();
    forNode->setBlock(blockNode);

    next("end"); // end
    leave();
    return forNode;
}

/* for namelist in explist do block end | */
ASTNode* Parser::range_for_stat() {
    enter("range_for");
    next("for"); // for
    ASTNode* forNode = new ASTNode;

    ASTNode* namelistNode = namelist();
    forNode->addChild(namelistNode);

    next("in"); // in

    ASTNode* rangeNode = explist();
    forNode->addChild(rangeNode);

    next("do"); // do

    ASTNode* blockNode = block();
    forNode->addChild(blockNode);

    next("end"); // end

    leave();
    return forNode;
}

ASTNode* Parser::function_stat() {
    next("function");
    ASTNode* node = new ASTNode;
    //funcname
    node->addChild(funcname());
    node->addChild(funcbody());
    leave();
    return node;
}

/* local function Name funcbody */
ASTNode* Parser::local_function_stat() {
    enter("local_function");
    next("local");
    next("function");

    ASTNode* functionNode = new ASTNode;
    NameNode* nameNode = name();

    ASTNode* funcbodyNode = funcbody();
    functionNode->addChild(nameNode);
    functionNode->addChild(funcbodyNode);

    leave();
    return functionNode;
}

/* local namelist [‘=’ explist] */
ASTNode* Parser::local_namelist_stat() {
    enter("local_namelist");
    next("local");
    ASTNode* namelistNode = namelist();
    if(match(Token::ASSIGN)) {
        next(Token::ASSIGN);
        ASTNode* explistNode = explist();
        namelistNode->addChild(explistNode);
    }
    leave();
    return namelistNode;
}


/*
 * stat ::=  ‘;’ | 
 *      //  varlist ‘=’ explist | 
 *      //  functioncall | 
 *      // label | 
 *      // break | 
 *      // goto Name | 
 *      // do block end | 
 *      // while exp do block end | 
 *      // repeat block until exp | 
 *      //  if exp then block {elseif exp then block} [else block] end | 
 *      // for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end | 
 *      // for namelist in explist do block end | 
 *      //  function funcname funcbody | 
 *      // local function Name funcbody | 
 *      // local namelist [‘=’ explist] 
 */
ASTNode* Parser::stat() {
    if(current() == NULL)
        return NULL;

    enter("stat");


    if(match(Token::COMMENT)) {
        next();
        leave();
        return empty();
    } else if(match(Token::SEMICOLON) ) {
        next();
        leave();
        return empty();
    } else if( match ("function") ) {
        return function_stat();
    } else if( match ("if") ) {
        return if_stat();
    } else if( match ("for") ) {
        if (match(Token::ASSIGN, 2)) {  // varlist
            return for_stat();
        } else {
            return range_for_stat();
        }   
    } else if ( match ("while") ) {
        return while_stat();
    } else if ( match ("do") ) {
        return do_stat();
    } else if(match("break")) {
        NameNode* node = name();
        return node;
    } else if ( match (Token::LABEL) ) {
        return label();
    } else if ( match ("repeat") ) {
        return repeat_stat();
    } else if ( match ("goto")) {
        return goto_stat();
    } else if(match("end") 
            || match("return") 
            || match("elseif") 
            || match("until") 
            || match(Token::ASSIGN) ) {
        leave();
        return NULL; // 特殊处理
    } else if (match("local") ) {
        if( match("function", 1) )  {
            return local_function_stat();
        } else {
            return local_namelist_stat();
        }
    } 



    if( match(Token::ID) || match(Token::LP) ) {
        ASTNode* varNode = var();

        if(!match(Token::COMMA) && !match(Token::ASSIGN)) {
            leave();
            return varNode;
        }else{

            AssignNode* assignNode = new AssignNode;
            assignNode->addVar(varNode);


            while (match(Token::COMMA)) {
                next();  // eat ,
                ASTNode* nextVarNode = var();
                assignNode->addVar(nextVarNode);
            }

            next(Token::ASSIGN); 
            ASTNode* explistNode = explist();

            for(int i = 0; i < explistNode->children_count(); i++) {
                assignNode->addExp(explistNode->children(i));
            }
            delete explistNode; //todo

            leave();
            return assignNode;
        }
    }

}

/*
 * retstat ::= return [explist] [‘;’]
 */
ASTNode* Parser::retstat() {
    enter("retstat");

    next("return"); // eat key word 'return'

    ASTNode* explistNode = explist();
    ASTNode* retStatNode = new ASTNode;
    retStatNode->addChild(explistNode);

    if (match(Token::SEMICOLON)) {
        next(Token::SEMICOLON);
    }

    leave();
    return retStatNode;
}


/*
 * label ::= ‘::’ Name ‘::’
 */
ASTNode* Parser::label() {
    enter("label");
    next(Token::LABEL);

    ASTNode* labelNode = new ASTNode;
    labelNode->addChild(name());
    next(Token::ID);
    next(Token::LABEL);

    leave();
    return labelNode;
}

/*
 * funcname ::= Name {‘.’ Name} [‘:’ Name]
 */
ASTNode* Parser::funcname() {
    enter("funcname");
    ASTNode* node = new ASTNode();
    node->addChild(name());

    while(match(Token::DOT)) {
        next();
        node->addChild(name());
    }
    if( match(Token::COLON) ) {
        next();
        node->addChild(name());
    }

    leave();
    return node;
}

/*
 * varlist ::= var {‘,’ var}
 */
ASTNode* Parser::varlist() {
    enter("varlist");

    ASTNode* varlistNode = new ASTNode;
    ASTNode* varNode = var();
    varlistNode->addChild(varNode);

    while(match(Token::COMMA)) {
        next();
        ASTNode* varNode = var();
        varlistNode->addChild(varNode);
    }

    leave();
    return varlistNode;
}

/*
 * // var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name 
 *
 * var ::= Name `var | ( exp ) `prefixexp [ exp ] `var | ( exp ) `prefixexp . Name `var
 */
ASTNode* Parser::var() {
    enter("var");

    ASTNode* node = NULL;
    if(match(Token::ID) ) {

        node = name();

    } else if(match(Token::LP)) {
        node = new ASTNode;

        next(Token::LP);
        ASTNode* expNode = exp();
        node->addChild(expNode);

        _prefixexp(node);

        if(match(Token::LBRACKET)) {
            next(Token::LBRACKET);
            ASTNode* expNode2 = exp();
            node->addChild(expNode2);
            next(Token::RBRACKET);

        }
        else if (match(Token::DOT)) {
            next(Token::DOT);
            //next is Name
            node->addChild(name());
        }
    }
    _var(node);

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
        next();
        prefix->addChild(name());
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

    NameNode* nameNode = name();

    namelistNode->addChild(nameNode);

    while(match(Token::COMMA)) {
        next();

        NameNode* nameNode = name();

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
    ASTNode* expNode = exp();
    explistNode->addChild(expNode);
    while(match(Token::COMMA) && !match("...", 1) ){
        next(Token::COMMA);
        ASTNode* expNode = exp();
        explistNode->addChild(expNode);
    }
    leave();
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

    if(match("end")) {
        return NULL; // 特殊处理
    }

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
        LeafNode* node = leaf();
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
            UnopNode* testNode = unop();
            if(testNode) { // is unop
                expNode->addChild( testNode );
                ASTNode* expNode2 = exp();
                expNode->addChild( expNode2 );

            } else {
                ASTNode* prefixExpNode = prefixexp();
                expNode->addChild(prefixExpNode);
            }
        }
    }
    _exp(expNode);

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
        next();
        prefix->addChild(name());
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
        next(Token::COLON);
        node->addChild(name());
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
        next(Token::LP);
        if(!match(Token::RP)) {
            ASTNode* explistNode = explist();
            next(Token::RP);
            leave();
            return explistNode;
        } else {
            next(Token::RP);
            leave();
            return NULL;
        }
    } else if ( match(Token::STRING) ) {
        LeafNode *argsNode = leaf();
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

    ASTNode* blockNode = block();

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
        leave();
        return NULL;
    } else if(match("...")) {
        LeafNode* node =  leaf();
        namelistNode->addChild(node);
        leave();
        return namelistNode;
    } else {
        namelistNode = namelist();
        if(match(Token::COMMA)) {
            next();
            namelistNode->addChild(leaf());
        }
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
    leave();
    return node;
}

/*
 * field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
 */
ASTNode* Parser::field() {
    enter("field");
    FieldNode* fieldNode = new FieldNode;

    if (match(Token::LBRACKET) ) {
        next(Token::LBRACKET);
        ASTNode* keyNode = exp();
        next(Token::ASSIGN);
        ASTNode* valueNode = exp();
        fieldNode->setKey(keyNode);
        fieldNode->setKey(valueNode);
    }
    else if (match(Token::ID) && match(Token::ASSIGN, 1)) {
        NameNode* keyNode = name();
        next(); // =
        ASTNode* valueNode = exp();
        fieldNode->setKey(keyNode);
        fieldNode->setValue(valueNode);
    } else {
        ASTNode* keyNode = exp();
        fieldNode->setKey(keyNode);
    }
    leave();
    return fieldNode;
}

/*
 * fieldsep ::= ‘,’ | ‘;’
 */
bool Parser::fieldsep() {
    enter("fieldsep");
    if(match(Token::SEMICOLON) || match (Token::COMMA)) {
        next();
        return true;
    }
    leave();
    return false;
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
            match(Token::DOTDOT) ||
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
                BinopNode* binopNode = new BinopNode(current());
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
UnopNode* Parser::unop() {
    enter("unop");
    if(match(Token::SUB) ||
            match(Token::SHARP) ||
            match(Token::NOT) ||
            match("not")
      )
    {
        UnopNode* unopNode = new UnopNode(current());
        next();
        leave();
        return unopNode;
    }
    leave();
    return NULL;
}

NameNode* Parser::name() {
    NameNode* node = new NameNode(current());
    next();
    return node;
}

LeafNode* Parser::leaf() {
    LeafNode* leafNode = new LeafNode(current());
    next();
    return leafNode;
}

EmptyNode* Parser::empty() {
    return new EmptyNode;
} 

