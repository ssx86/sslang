#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"

class Value {
};

class ASTNode{
    public:
        Token* op;
        ASTNode* children(int i);
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;
        Value* eval() {
        };
};


class EmptyNode : public ASTNode {
};

#endif
