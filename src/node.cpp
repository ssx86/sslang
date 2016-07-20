#include "node.h"

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

/*********************************
 * ASTNode
 * *******************************/

void ASTNode::addChild(ASTNode* child) {
    m_children.push_back(child);
}

ASTNode* ASTNode::children(int i) {
    return m_children[i];
}


ASTNode::~ASTNode() {
}

int ASTNode::children_count() {
    return m_children.size();
}

std::string ASTNode::name() {
    return "ASTNode Name UNKNOWN";
}

/*********************************
 * RetNode
 * *******************************/

Value* RetNode::eval(Enveronment* env) {
    Value* value = NULL;
    for(int i = 0; i < m_children.size(); i++) {
        value = children(i)->eval(env);
    }
    return value;
}

/*********************************
 * BlockNode
 * *******************************/

Value* BlockNode::eval(Enveronment* env) {
    if (!env) 
        env = new Enveronment;

    Value* value = NULL;
    for(int i = 0; i < m_children.size(); i++) {

        value = children(i)->eval(env);
        if( dynamic_cast<RetNode*>( children(i) ) ) {
            break;
        }
    }
    //assert(value);
    return value;
}

/*********************************
 * NameNode
 * *******************************/

NameNode::NameNode(Token* token) {
    m_name = token->tostring();
}

Value* NameNode::eval(Enveronment* env) {
    return env->get(m_name);
}

std::string NameNode::name() {
    return m_name;
}

/*********************************
 * LeafNode
 * *******************************/

LeafNode::LeafNode() { //TODO value nil!!!
    m_value = new StringValue("nil");
}

LeafNode::LeafNode(std::string s) {
    m_value = new StringValue(s);
}

LeafNode::LeafNode(int i) {
    m_value = new IntValue(i);
}

LeafNode::LeafNode(double d) {
    m_value = new DoubleValue(d);
}

LeafNode::LeafNode(bool b) {
    m_value = new BoolValue(b);
}

Value* LeafNode::eval(Enveronment* env) {
    return m_value;
}

/*********************************
 * FieldNode
 * *******************************/

FieldNode::FieldNode() : 
    m_key(NULL),
    m_value(NULL) {
    }

void FieldNode::setKey(ASTNode* key) {
    m_key = key;
}

ASTNode* FieldNode::getKey() {
    return m_key;
}

ASTNode* FieldNode::getValue() {
    return m_value;
}

void FieldNode::setValue(ASTNode* value) {
    m_value = value;
}

Value* FieldNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    return new StringValue("<FieldNode>");
}


/*********************************
 * BinExpNode
 * *******************************/

BinExpNode::BinExpNode(Token* token) : m_left(NULL), m_right(NULL) {
    m_op = token;
}

void BinExpNode::setLeft(ASTNode* left) {
    m_left = left;
}

void BinExpNode::setRight(ASTNode* right) {
    m_right = right;
}

Token* BinExpNode::getOp() {
    return m_op;
}

ASTNode* BinExpNode::getLeft() {
    return m_left;
}

ASTNode* BinExpNode::getRight() {
    return m_right;
}

Value* BinExpNode::eval(Enveronment* env) {
    Value* left = m_left->eval(env);
    Value* right = m_right->eval(env);

    if(m_op->isType(Token::ADD)) {
        return BinExpEval::AddOp(left, right);
    } else if(m_op->isType(Token::SUB)) {
        return BinExpEval::SubOp(left, right);
    } else if(m_op->isType(Token::MUL)) {
        return BinExpEval::MulOp(left, right);
    } else if(m_op->isType(Token::DIV)) {
        return BinExpEval::DivOp(left, right);
    } else if(m_op->isType(Token::EQ)) {
        return BinExpEval::EqOp(left, right);
    } else if(m_op->isType(Token::GT)) {
        return BinExpEval::GTOp(left, right);
    } else if(m_op->isType(Token::GE)) {
        return BinExpEval::GEOp(left, right);
    } else if(m_op->isType(Token::LT)) {
        return BinExpEval::LTOp(left, right);
    } else if(m_op->isType(Token::LE)) {
        return BinExpEval::LEOp(left, right);
    } else if(m_op->isType(Token::NE)) {
        return BinExpEval::NEOp(left, right);
    } else if(m_op->isType(Token::DOTDOT)) {
        return BinExpEval::ConcatOp(left, right);
    } else if(m_op->isKey("and")) {
        return BinExpEval::AndOp(left, right);
    } else if(m_op->isKey("or")) {
        return BinExpEval::OrOp(left, right);
    } else {
        return new IntValue(0);
    }
}

/*********************************
 * ForNode
 * *******************************/

ForNode::ForNode() : 
    m_from(NULL),
    m_to(NULL),
    m_step(NULL) {
    }

void ForNode::setName(NameNode* name) {
    m_name = name;
}

void ForNode::setFrom(ASTNode* exp) {
    m_from = exp;
}

void ForNode::setTo(ASTNode* exp) {
    m_to = exp;
}

void ForNode::setStep(ASTNode* exp) {
    m_step = exp;
}

void ForNode::setBlock(ASTNode* block) {
    m_block = block;
}

Value* ForNode::eval(Enveronment* env) {

    // 设置参数环境
    Enveronment *localEnv = new Enveronment;
    localEnv->next = env;

    Value* value = NULL;
    int from = m_from->eval(env)->intValue();
    int to = m_to->eval(env)->intValue();

    env->add(m_name->name(), new IntValue(from));


    while( env->get(m_name->name())->intValue() <= to ) {
        int step = 1;
        if(m_step) {
            step = m_step->eval(env)->intValue();
        }

        from += step;
        to = m_to->eval(env)->intValue();

        value = m_block->eval(localEnv);

        env->update(m_name->name(), new IntValue(from));
    }
    //assert(value);
    return value;
}

/*********************************
 * EmptyNode
 * *******************************/

Value* EmptyNode::eval(Enveronment* env) {
    return new StringValue("empty");
}

/*********************************
 * TableAccessNode
 * *******************************/

void TableAccessNode::setTable(ASTNode* table) {
    m_table = table;
}
void TableAccessNode::setIndex(ASTNode* index) {
    m_index = index;
}

Value* TableAccessNode::eval(Enveronment* env) {
    //TODO here is wrong!
    Value* vIndex = m_index->eval(env);
    if (StringValue* sname = dynamic_cast<StringValue*>(vIndex)) { // map

        std::string name = sname->stringValue();

        TableValue* table = dynamic_cast<TableValue*>(m_table->eval(env));

        return table->getMapValue(name);
    } else {  // array
        Value* vIndex = m_index->eval(env);
        if (! dynamic_cast<NilValue*>(vIndex) ) {
            int index = vIndex->intValue();
            TableValue* table = dynamic_cast<TableValue*>(m_table->eval(env));
            return table->getArrayValue(index);
        } else {
            return new NilValue;
        }
    }
}

Value*& TableAccessNode::lvalue(Enveronment* env) {
    Value* vIndex = m_index->eval(env);
    if (StringValue* sname = dynamic_cast<StringValue*>(vIndex)) { // map
        std::string name = sname->stringValue();

        TableValue* table = dynamic_cast<TableValue*>(m_table->eval(env));

        return table->getMaplValue(name);
    } else {  // array
        Value* vIndex = m_index->eval(env);
        if (! dynamic_cast<NilValue*>(vIndex) ) {
            int index = vIndex->intValue();
            TableValue* table = dynamic_cast<TableValue*>(m_table->eval(env));
            return table->getArraylValue(index);
        } else {
            cerr << "table index is nil" << endl;
            exit(1);
        }
    }
}


/*********************************
 * AssignNode
 * *******************************/

void AssignNode::addVar(ASTNode* var) {
    m_vars.push_back(var);
}

void AssignNode::addExp(ASTNode* exp) {
    m_exps.push_back(exp);
}

Value* AssignNode::eval(Enveronment* env) {
    Value* value = NULL;
    for(int i = 0; i < (int)m_vars.size(); i++) {
        NameNode* pName = dynamic_cast<NameNode*>(m_vars[i]);
        if(pName) {
            std::string varName = pName->name();
            value = m_exps[i]->eval(env);
            if (! env->update(varName, value)) {  //先检索祖先环境链。这是和local赋值的唯一区别
                env->add(varName, value);
            }
        } else {

            TableAccessNode* pTableAccess = dynamic_cast<TableAccessNode*>(m_vars[i]);
            if (pTableAccess) {
                Value*& lvalue = pTableAccess->lvalue(env);
                value = m_exps[i]->eval(env);
                lvalue = value;
            } else {
                std::cout << "error reading var name" << std::endl;
            }
        }

    }
    return value;
}

/*********************************
 * FieldListNode
 * *******************************/

Value* FieldListNode::eval(Enveronment* env) {
    return NULL; // never eval
}

/*********************************
 * NameListNode
 * *******************************/

Value* NameListNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<NameListNode>");
    return value;
}

/*********************************
 * FuncBodyNode
 * *******************************/

FuncBodyNode::FuncBodyNode() 
    : m_block(NULL), m_parlist(NULL) {
    }

void FuncBodyNode::setBlock(BlockNode* block) {
    m_block = block;
}

BlockNode* FuncBodyNode::getBlock() {
    return m_block;
}

void FuncBodyNode::setParList(NameListNode* parlist) {
    m_parlist = parlist;
}

NameListNode* FuncBodyNode::getParList() {
    return m_parlist;
}

Value* FuncBodyNode::eval(Enveronment* env) {
    return m_block->eval(env);
}

/*********************************
 * FuncCallNode
 * *******************************/

void FuncCallNode::setArgs(ASTNode* args) {
    m_args = args;
}

void FuncCallNode::setName(ASTNode* name) {
    m_name = name;
}


Value* FuncCallNode::eval(Enveronment* env) {
    FuncValue* funcValue = NULL;
    if( NameNode* nameNode = dynamic_cast<NameNode*>(m_name) ) {
        if ( nameNode->name() == "print" ) {
            Value* value = m_args->children(0)->eval(env);
            cout << value->tostring() << endl;
            return value;
        } else {
            funcValue = dynamic_cast<FuncValue*>(env->get(m_name->name()));
        }
    } else {
        if (! dynamic_cast<TableAccessNode*>(m_name) 
                && ! dynamic_cast<FunctionDefNode*>(m_name) 
                && ! dynamic_cast<FuncCallNode*>(m_name) ) {
            std::cout << "fuck wrong!!!" << std::endl;
            exit(1);
        }

        funcValue = dynamic_cast<FuncValue*>(m_name->eval(env));
    }

    if (funcValue) {
        NameListNode* parList = funcValue->getBodyNode()->getParList();

        // 设置参数环境
        Enveronment *localEnv = new Enveronment;
        localEnv->next = env;

        for(int i = 0; i < parList->children_count(); i++) {
            std::string name = parList->children(i)->name();
            localEnv->add(name, m_args->children(i)->eval(env));
        }

        return funcValue->call(localEnv);
    } else {
        cout << "error, not found function: " << (dynamic_cast<NameNode*>(m_name))->name() << "->" << m_name->eval(env)->type() << endl;
        exit(1);
        return NULL;

    }
}

/*********************************
 * ExpListNode
 * *******************************/

Value* ExpListNode::eval(Enveronment* env) {
    Value* value = NULL;
    for(int i = 0; i < m_children.size(); i++) {
        value = children(i)->eval(env);
    }
    assert(value);
    return value;
}

/*********************************
 * VarNode
 * *******************************/

Value* VarNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<VarNode>");
    return value;
}

/*********************************
 * VarListNode
 * *******************************/

Value* VarListNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<VarListNode>");
    return value;
}



/*********************************
 * LabelNode
 * *******************************/

Value* LabelNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<LabelNode>");
    return value;
}

/*********************************
 * FunctionNode
 * *******************************/

void FunctionNode::setName(ASTNode* name) {
    m_name = name;
}

void FunctionNode::setBody(FuncBodyNode* body) {
    m_body = body;
}

Value* FunctionNode::eval(Enveronment* env) {
    NameNode* pName = dynamic_cast<NameNode*>(m_name);
    if(pName) {
        env->add(m_name->name(), new FuncValue(m_body));
    } else {
        TableAccessNode* pTableAccess = dynamic_cast<TableAccessNode*>(m_name);
        if (pTableAccess) {
            Value*& lvalue = pTableAccess->lvalue(env);
            lvalue = new FuncValue(m_body);
        } else {
            std::cout << "error reading var name" << std::endl;
        }
    }
    return new StringValue("<FunctionNode>");
}

/*********************************
 * CondNode
 * *******************************/

void CondNode::setCondition(ASTNode* cond) {
    m_condition = cond;
}

void CondNode::setValue(ASTNode* value) {
    m_value = value;
}

bool CondNode::getBool(Enveronment* env) {
    if(!m_condition)
        return true;

    return m_condition->eval(env)->boolValue();
}

Value* CondNode::getValue(Enveronment* env) {
    return m_value->eval(env);
}

/*********************************
 * IfNode
 * *******************************/

void IfNode::addCond(CondNode* cond) {
    m_conds.push_back(cond);
}

Value* IfNode::eval(Enveronment* env) {
    for(int i = 0; i < m_conds.size(); i++) {
        if( m_conds[i]->getBool(env) )
            return m_conds[i]->getValue(env);
    }
    Value* value = new StringValue("<IfNode>");
    return value;
}

/*********************************
 * RepeatNode
 * *******************************/

Value* RepeatNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<RepeatNode>");
    return value;
}

/*********************************
 * WhileNode
 * *******************************/

Value* WhileNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<WhileNode>");
    return value;
}

/*********************************
 * DoNode
 * *******************************/

Value* DoNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<DoNode>");
    return value;
}

/*********************************
 * GotoNode
 * *******************************/

Value* GotoNode::eval(Enveronment* env) {
    for(int i = 0; i < m_children.size(); i++) {
        std::cout << children(i)->eval(env)->tostring() << std::endl;
    }
    Value* value = new StringValue("<GotoNode>");
    return value;
}

/*********************************
 * BinOpHolderNode
 * *******************************/

BinOpHolderNode::BinOpHolderNode(Token* op) {
    m_op = op;
}

Token* BinOpHolderNode::token() {
    return m_op;
}

int BinOpHolderNode::priority() {
    return m_op->pri();
}

Value* BinOpHolderNode::eval(Enveronment* env) {
    return NULL; // never eval
}

/*********************************
 * FunctionDefNode
 * *******************************/

void FunctionDefNode::setBody(FuncBodyNode* body) {
    m_body = body;
}

Value* FunctionDefNode::eval(Enveronment* env) {
    return new FuncValue(m_body, env);
}

/*********************************
 * UnopExpNode
 * *******************************/

UnopExpNode::UnopExpNode(Token* op) {
    m_op = op;
}

void UnopExpNode::setExp(ASTNode* exp) {
    m_exp = exp;
}

Value* UnopExpNode::eval(Enveronment* env) {
    switch(m_op->type()) {
        case Token::SUB:
            return BinExpEval::MulOp(new IntValue(-1), m_exp->eval(env));
        case Token::ID:
            if(m_op->isKey("not")) {
                return new BoolValue( ! m_exp->eval(env)->boolValue() );
            } else {
                cout << "error unop" << endl;
                exit(1);
            }
        case Token::SHARP:
            {
                TableValue* vTable = dynamic_cast<TableValue*>(m_exp->eval(env));
                if (vTable) {
                    return new IntValue(vTable->getLength());
                } else {
                    cout << "error not table" << endl;
                    exit(1);
                }
            }
        case Token::NOT:
            return new BoolValue( ! m_exp->eval(env)->boolValue() );
        default:
            exit(1);
    }
}

/*********************************
 * TableNode
 * *******************************/

void TableNode::setField(FieldListNode* fieldlist) {
    m_fieldlist = fieldlist;
}

Value* TableNode::eval(Enveronment* env) {
    TableValue* table = new TableValue();
    int pos = 1;
    for(int i = 0; i < m_fieldlist->children_count(); i++) {
        FieldNode* fieldNode = dynamic_cast<FieldNode*>(m_fieldlist->children(i));
        if ( ASTNode* key = fieldNode->getKey() ) {
            // map
            if ( NameNode* name = dynamic_cast<NameNode*>(key) ) {
                table->setMapValue(name->name(), fieldNode->getValue()->eval(env));
            } else {
                int index = key->eval(env)->intValue();
                table->setArrayValue(index, fieldNode->getValue()->eval(env));
                pos = index + 1;
            }
        } else {
            table->setArrayValue(pos, fieldNode->getValue()->eval(env));
            pos++;
        }
    }
    return table;
}

