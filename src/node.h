#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include <iostream>

class Value {
};

class ASTNode{
    public:
        Token* op;
        ASTNode* children(int i);
        int children_count() {
            return m_children.size();
        }
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;
        virtual Value* eval() {
            Value* val = NULL;
            for(std::vector<ASTNode*>::iterator it =  m_children.begin(); it != m_children.end(); 
                    it++)
            {
                val = (*it)->eval();
            }
            return val;
        }
};

class RootNode : public ASTNode {
};

class BlockNode : public ASTNode {
};

class StatNode : public ASTNode {
};

class RetStatNode : public ASTNode {
};

class EmptyStatNode : public StatNode {
    public:
        Value* eval() {
            std::cout << "here is an empty stat node" << std::endl;
            return NULL;
        }
};

#endif
