#include "node.h"
void ASTNode::addChild(ASTNode* child) {
    m_children.push_back(child);
}

ASTNode* ASTNode::children(int i) {
    return m_children[i];
}


Value* execute(Token* op, Value* left, Value* right, Value::Type retType)
{
    if(op->isType(Token::ADD)) {
        switch (retType)
        {
            case Value::INT:
                return new IntValue(left->intValue() + right->intValue());
            case Value::DOUBLE:
                return new DoubleValue(left->doubleValue() + right->doubleValue());
        }
    } else if(op->isType(Token::SUB)) {
        switch (retType)
        {
            case Value::INT:
                return new IntValue(left->intValue() - right->intValue());
            case Value::DOUBLE:
                return new DoubleValue(left->doubleValue() - right->doubleValue());
        }
    } else if(op->isType(Token::MUL)) {
        switch (retType)
        {
            case Value::INT:
                return new IntValue(left->intValue() * right->intValue());
            case Value::DOUBLE:
                return new DoubleValue(left->doubleValue() * right->doubleValue());
        }
    } else if(op->isType(Token::DIV)) {
        switch (retType)
        {
            case Value::INT:
                return new IntValue(left->intValue() / right->intValue());
            case Value::DOUBLE:
                return new DoubleValue(left->doubleValue() + right->doubleValue());
        }
    }
}
