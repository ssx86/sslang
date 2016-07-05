#include "parser.h"
#include "lexer.h"
#include "token.h"

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <stack>

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
    : level(-1), m_root(NULL) {
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
            //std::cout << "end of file" << std::endl;
            break;
        }
    } while (m_current->isType(Token::NEWLINE) || m_current->isType(Token::COMMENT) );

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
    m_root = chunk();
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
BlockNode* Parser::chunk() {
    BlockNode* node =  block();
    return node;
}

/*
 * block ::= {stat} [retstat]
 */
BlockNode* Parser::block() {
    if (current() == NULL) 
        return NULL;
    enter("block");

    BlockNode* node = new BlockNode;
    if (!match("return")) {
        while( ASTNode* stat_node = stat() ) {
            node->addChild(stat_node);
        }
    }
    if (match("return")) /* TODO: retstat */
    {
        RetNode* retstat_node = retstat();
        node->addChild(retstat_node);
    }
    leave();
    return node;
}


/* goto Name */
GotoNode* Parser::goto_stat() {
    next("goto");
    GotoNode* gotoNode = new GotoNode;
    NameNode* nameNode = name();

    gotoNode->addChild(nameNode);
    return gotoNode;
}

/*      // do block end  */
DoNode* Parser::do_stat() {
    enter("do");
    DoNode* doNode = new DoNode;
    next("do");

    BlockNode* blockNode = block();
    next("end");

    doNode->addChild(blockNode);

    leave();
    return doNode;
}

/* while exp do block end | */
WhileNode* Parser::while_stat() {
    next("while"); // while
    WhileNode* whileNode = new WhileNode;
    ASTNode* expNode = exp();

    next("do"); // do

    BlockNode* blockNode = block();

    next("end"); // end

    whileNode->addChild(expNode);
    whileNode->addChild(blockNode);

    return whileNode;
}

/* repeat block until exp */
RepeatNode* Parser::repeat_stat() {
    enter("repeat");

    next("repeat");
    RepeatNode* repeatNode = new RepeatNode;
    BlockNode* blockNode = block();
    next("until");
    ASTNode* expNode = exp();
    repeatNode->addChild(blockNode);
    repeatNode->addChild(expNode);

    leave();
    return repeatNode;
}

IfNode* Parser::if_stat() {
    enter("if");

    IfNode* ifNode = new IfNode;
    next("if");

    CondNode* pathNode = new CondNode;

    ASTNode* expNode = exp();
    next("then"); // then
    BlockNode* blockNode = block();
    pathNode->setCondition(expNode);
    pathNode->setValue(blockNode);

    ifNode->addCond(pathNode);

    while(match("elseif")) {
        next("elseif");
        CondNode* pathNode = new CondNode;

        ASTNode* expNode = exp();
        next("then"); // then
        BlockNode* blockNode = block();
        pathNode->setCondition(expNode);
        pathNode->setValue(blockNode);

        ifNode->addCond(pathNode);
    }
    if(match("else")) {
        next("else");
        CondNode* pathNode = new CondNode;

        pathNode->setCondition(NULL);
        ASTNode* blockNode = block();

        pathNode->setValue(blockNode);

        ifNode->addCond(pathNode);
    }

    next("end"); // "end"

    leave();
    return ifNode;
}

/* for Name ‘=’ exp ‘,’ exp [‘,’ exp] do block end |  */
ForNode* Parser::for_stat() {
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

    BlockNode* blockNode = block();
    forNode->setBlock(blockNode);

    next("end"); // end
    leave();
    return forNode;
}

/* for namelist in explist do block end | */
ForNode* Parser::range_for_stat() {
    enter("range_for");
    next("for"); // for
    ForNode* forNode = new ForNode;

    NameListNode* namelistNode = namelist();
    forNode->addChild(namelistNode);

    next("in"); // in

    ExpListNode* rangeNode = explist();
    forNode->addChild(rangeNode);

    next("do"); // do

    BlockNode* blockNode = block();
    forNode->addChild(blockNode);

    next("end"); // end

    leave();
    return forNode;
}

FunctionNode* Parser::function_stat() {
    next("function");
    FunctionNode* node = new FunctionNode;
    //funcname
    node->setName(funcname());
    node->setBody(funcbody());
    leave();
    return node;
}

/* local function Name funcbody */
ASTNode* Parser::local_function_stat() {
    enter("local_function");
    next("local");
    next("function");

    FunctionNode* functionNode = new FunctionNode;
    NameNode* nameNode = name();

    FuncBodyNode* funcbodyNode = funcbody();
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
            ExpListNode* explistNode = explist();

            for(int i = 0; i < explistNode->children_count(); i++) {
                assignNode->addExp(explistNode->children(i));
            }
            delete explistNode; //todo

            leave();
            return assignNode;
        }
    }
    leave();
    return NULL;

}

/*
 * retstat ::= return [explist] [‘;’]
 */
RetNode* Parser::retstat() {
    enter("retstat");

    next("return"); // eat key word 'return'

    RetNode* retStatNode = new RetNode;

    ExpListNode* explistNode = explist();
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
LabelNode* Parser::label() {
    enter("label");
    next(Token::LABEL);

    LabelNode* labelNode = new LabelNode;
    labelNode->addChild(name());
    next(Token::ID);
    next(Token::LABEL);

    leave();
    return labelNode;
}

/*
 * funcname ::= Name {‘.’ Name} [‘:’ Name]
 */
FuncNameNode* Parser::funcname() {
    enter("funcname");
    FuncNameNode* node = new FuncNameNode();
    node->setName(name());

    while(match(Token::DOT)) {
        next();
        node->appendName(name());
    }
    if( match(Token::COLON) ) {
        next();
        node->appendName(name());
    }

    leave();
    return node;
}

/*
 * varlist ::= var {‘,’ var}
 */
VarListNode* Parser::varlist() {
    enter("varlist");

    VarListNode* varlistNode = new VarListNode;
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
        node = new VarNode;

        next(Token::LP);
        ASTNode* expNode = exp();
        node->addChild(expNode);

        node = _prefixexp(node);

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
    node = _var(node);

    leave();
    return node;
}

/*
 * `var ::= `prefixexp [ exp ] `var | `prefixexp . Name `var | nil
 */
ASTNode* Parser::_var(ASTNode* prefix) {
    enter("_var");
    prefix = _prefixexp(prefix);

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
        return prefix;
    }

    prefix = _var(prefix);

    leave();
    return prefix;
}

/*
 * namelist ::= Name {‘,’ Name}
 */
NameListNode* Parser::namelist() {
    enter("namelist");

    NameListNode* namelistNode = new NameListNode;

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
ExpListNode* Parser::explist() {
    enter("explist");

    ExpListNode* explistNode = new ExpListNode;
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
ASTNode* Parser::singleExp() {
    enter("exp");

    if(match("end") || match(Token::COMMENT)) {
        return NULL; // 特殊处理
    }

    ASTNode* expNode = NULL;
    LeafNode* leafNode = leaf();
    if (leafNode) { // nil | false | true | Numeral | LiteralString | '...'
        expNode = leafNode;
    } else {
        // functiondef, prefixexp, tableconstructor, unop
        if (match(Token::LB)) {
            //table
            ASTNode* tableNode = tableconstructor();
            expNode = tableNode;
        } else if (match("function")) {
            //func
            ASTNode* functiondefNode = functiondef();
            expNode = functiondefNode;
        } 
        // unop
        else {
            UnopNode* testNode = unop();
            if(testNode) { // is unop
                expNode = testNode ;
                ASTNode* expNode2 = exp();
                expNode->addChild( expNode2 );

            } else {
                ASTNode* prefixExpNode = prefixexp();
                expNode = prefixExpNode;
            }
        }
    }

    leave();
    return expNode;
}
ASTNode* Parser::exp() {
    std::vector<ASTNode*> nodes;
    ASTNode* exp1 = singleExp();
    nodes.push_back(exp1);

    while(isBinop()) {
        BinOpHolderNode* op = new BinOpHolderNode(binop());
        ASTNode* exp2 = singleExp();
        nodes.push_back(op);
        nodes.push_back(exp2);
    }


    // 中缀转后缀
    std::stack<BinOpHolderNode*> opstack;
    std::vector<ASTNode*> output_nodes;
    for(int i = 0; i < nodes.size(); i++) {
        if(BinOpHolderNode* op = dynamic_cast<BinOpHolderNode*>(nodes[i])) {
            if (opstack.empty() ) {
                    opstack.push(op);
            } else if( opstack.top()->priority() <= op->priority() ) {
                    opstack.push(op);
            } else {
                while( !opstack.empty() && opstack.top()->priority() >= op->priority() ) {
                    BinOpHolderNode* temp = opstack.top();
                    output_nodes.push_back(temp);
                    opstack.pop();
                }
                opstack.push(op);
            }
        } else {
            output_nodes.push_back(nodes[i]);
        }
    }
    while(!opstack.empty()) {
        ASTNode* temp = opstack.top();
        output_nodes.push_back(temp);
        opstack.pop();
    }
    std::stack<ASTNode*> tempstack;

    for(int i = 0; i < output_nodes.size(); i++ ) {
        if(BinOpHolderNode* op = dynamic_cast<BinOpHolderNode*>(output_nodes[i])) {
            ASTNode* right = tempstack.top(); tempstack.pop();
            ASTNode* left = tempstack.top(); tempstack.pop();
            BinExpNode* expNode = new BinExpNode(op->token());
            expNode->setLeft(left);
            expNode->setRight(right);
            tempstack.push(expNode);
        } else {
            tempstack.push(output_nodes[i]);
        }
    }
    assert(tempstack.size() == 1);
    ASTNode* finalNode = tempstack.top();
    tempstack.pop();
    return finalNode;
    

}

/* 
 * `exp ::= binop exp | nil
 */
ASTNode* Parser::_exp(ASTNode* prefix) {
    enter("_exp");

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
    node = _prefixexp(node);

    leave();
    return node;
}

/*
 * `prefixexp ::= args `prefixexp  | ':' Name args `prefixexp | ""
 */
ASTNode* Parser::_prefixexp(ASTNode* prefix) {
    enter("_prefixexp");

    if(current() == NULL)
        return prefix;

    if ( match (Token::COLON) ) {
        next();
        prefix->addChild(name());
        ASTNode* argsNode = args();
        prefix->addChild(argsNode);
        prefix = _prefixexp(prefix);
        leave();

        return prefix;
    } else {
        ASTNode* argsNode = args();
        if (argsNode) {
            FuncCallNode* funcCallNode = functioncall(prefix, argsNode);
            leave();
            return funcCallNode;
        }
        else
        {
            leave();
            return prefix;
        }
    }

}


/*
 * functioncall ::=  prefixexp args | prefixexp ‘:’ Name args 
 */
FuncCallNode* Parser::functioncall(ASTNode* prefixexpNode, ASTNode* argsNode) {
    enter("functioncall");

    FuncCallNode* node = new FuncCallNode;
    node->setName(prefixexpNode);
    node->setArgs(argsNode);

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
            ExpListNode* explistNode = explist();
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
FuncBodyNode* Parser::funcbody() {
    enter("funcbody");
    FuncBodyNode* node = new FuncBodyNode;

    next(Token::LP);
    NameListNode* parlistNode = parlist();

    next(Token::RP);
    BlockNode* blockNode = block();

    next(Token::ID); // "end"

    node->setParList(parlistNode);
    node->setBlock(blockNode);
    leave();
    return node ;
}

/*
 * parlist ::= namelist [‘,’ ‘...’] | ‘...’
 */
NameListNode* Parser::parlist() {
    enter("parlist");

    if(match(Token::RP)) {
        leave();
        return new NameListNode;
    } else if(match("...")) {
        NameNode* node = name();
        NameListNode* namelistNode = new NameListNode;
        namelistNode->addChild(node);
        leave();
        return namelistNode;
    } else {
        NameListNode* namelistNode = namelist();
        if(match(Token::COMMA)) {
            next();
            namelistNode->addChild(name());
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
FieldListNode* Parser::fieldlist() {
    enter("fieldlist");
    FieldListNode* node = new FieldListNode;

    FieldNode* fieldNode = field();
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
FieldNode* Parser::field() {
    enter("field");
    FieldNode* fieldNode = new FieldNode;

    if (match(Token::LBRACKET) ) {
        next(Token::LBRACKET);
        ASTNode* keyNode = exp();
        next(Token::ASSIGN);
        ASTNode* valueNode = exp();
        fieldNode->setKey(keyNode);
        fieldNode->setValue(valueNode);
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

bool Parser::isBinop() {
    if( match(Token::ADD) || 
            match(Token::SUB) ||
            match(Token::MUL) ||
            match(Token::DIV) ||
            //           match(Token:://) ||
            match(Token::XOR) ||
            match(Token::MOD) ||
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
            return true;
    else
        return false;
}


/*
 * binop ::=  ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘//’ | ‘^’ | ‘%’ | 
 * ‘&’ | ‘~’ | ‘|’ | ‘>>’ | ‘<<’ | ‘..’ | 
 * ‘<’ | ‘<=’ | ‘>’ | ‘>=’ | ‘==’ | ‘~=’ | 
 * and | or
 */
Token* Parser::binop() {
    enter("binop");
    Token* opToken = current();
    next();
    leave();
    return opToken;
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

    LeafNode* leafNode = NULL;
    if (match ("nil") ) {
        leafNode = new LeafNode();
    } else if( match("false")) {
        leafNode = new LeafNode(false);
    } else if (match("true")) {
        leafNode = new LeafNode(true);
    } else if (match(Token::STRING) ) {
        leafNode = new LeafNode(current()->svalue);
    } else if (match(Token::INT) ) {
        leafNode = new LeafNode(current()->ivalue);
    } else if (match(Token::DOUBLE) ) {
        leafNode = new LeafNode(current()->dvalue);
    } else if (match("...")) {
        leafNode = new LeafNode("...");
    } else {
        return NULL;
    }
    next();
    return leafNode;

}

EmptyNode* Parser::empty() {
    return new EmptyNode;
} 

