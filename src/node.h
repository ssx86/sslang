#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include <iostream>
#include <vector>

class Value {
};

class ASTNode{
    public:
        //member
        typedef enum{
           DUMMY 
        }Type;
        Type m_type;

        Token* m_token;

        //children
        ASTNode* children(int i);
        int children_count() {
            return m_children.size();
        }
        void addChild(ASTNode* child);

    public:
        ASTNode(Type type = DUMMY) : m_token(NULL), m_type(type) {}
        ASTNode(Token* token) {
            m_token = token;
        }
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


#endif
