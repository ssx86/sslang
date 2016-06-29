#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
using namespace std;

    

class Value {
    public:
    virtual std::string tostring() = 0;
};

class StringValue : public Value {
    public:
        StringValue(const char* str) {
            m_value = str;
        }
        StringValue(std::string str) {
            m_value = str;
        }

        std::string tostring() {
            return m_value;
        }
    private:
        std::string m_value;
};

class DoubleValue : public Value {
    public:
        DoubleValue(double d) {
            m_value = d;
        }
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%f", m_value);
            return std::string(temp);
        }
    private :
        double m_value;
};

class BoolValue : public Value {
    public:
        BoolValue(bool b) {
            m_value = b;
        }
        std::string tostring() {
            if(m_value) 
                return "true";
            else
                return "false";
        }
    private:
        bool m_value;
};

class IntValue : public Value {
    public:
        IntValue(int i) {
            m_value = i;
        }
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%d", m_value);
            return std::string(temp);
        }
    private :
        int m_value;
};

class Enveronment {
    public:
    map<std::string, Value*> value;
    Enveronment* next;
};

class ASTNode{
    public:

        //children
        ASTNode* children(int i);
        int children_count() {
            return m_children.size();
        }
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;

        virtual Value* eval(Enveronment* env = NULL) {

            if (! env) {
                env = new Enveronment;
            }

            Value* val = NULL;
            for(std::vector<ASTNode*>::iterator it =  m_children.begin(); it != m_children.end(); 
                    it++)
            {
                val = (*it)->eval(env);
            }
            return val;
        }
};


class NameNode : public ASTNode{
    public:
        NameNode(Token* token) {
            m_token = token;
        }
        virtual Value* eval(Enveronment* env = NULL) {
            return new StringValue(m_token->tostring());
        }
    private:
        Token* m_token;
};

class UnopNode : public ASTNode {
    public:
        UnopNode(Token* token) {
            m_op = token;
        }

    private:
        Token* m_op;
};

class LeafNode : public ASTNode {
    public:
        LeafNode(Token* token) {
            m_token = token;
        }
        virtual Value* eval(Enveronment* env = NULL) {
            return new StringValue(m_token->tostring());
        }
    private:
        Token* m_token;
};

class FieldNode : public ASTNode {
    public:
        FieldNode() : 
            m_key(NULL),
            m_value(NULL) {}

        void setKey(ASTNode* key) {
            m_key = key;
        }
        void setValue(ASTNode* value) {
            m_value = value;
        }
        virtual Value* eval(Enveronment* env = NULL) {
            return NULL;
        }
    private:
        ASTNode* m_key;
        ASTNode* m_value; // ExpNode
};

class BinopNode : public ASTNode {
    public:
        BinopNode(Token* token) {
            m_op = token;
        }

        virtual Value* eval(Enveronment* env = NULL) {
            return new StringValue("Binop");
        }
    private:
        Token* m_op;
};

class ForNode : public ASTNode {
    public:
        ForNode() : 
            m_from(NULL),
            m_to(NULL),
            m_step(NULL) {}

        void setName(NameNode* name) {
            m_name = name;
        }
        void setFrom(ASTNode* exp) {
            m_from = exp;
        }
        void setTo(ASTNode* exp) {
            m_to = exp;
        }
        void setStep(ASTNode* exp) {
            m_step = exp;
        }
        void setBlock(ASTNode* block) {
            m_block = block;
        }
        virtual Value* eval(Enveronment* env = NULL) {
            m_from->eval();
            m_to->eval();
            if(m_step) {
                m_step->eval();
            }
            return m_block->eval();
        }
    private:
        NameNode* m_name;
        ASTNode* m_from;
        ASTNode* m_to;
        ASTNode* m_step;

        ASTNode* m_block;
};

class EmptyNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env = NULL) {
            return new StringValue("empty");
        }
};

class AssignNode : public ASTNode {
    public:
        void addVar(ASTNode* var) {
            m_vars.push_back(var);
        }
        void addExp(ASTNode* exp) {
            m_exps.push_back(exp);
        }
        virtual Value* eval(Enveronment* env = NULL) {

            for(int i = 0; i < (int)m_vars.size(); i++) {

                env->value[m_vars[i]->eval()->tostring()] = m_exps[i]->eval();
                cout << m_vars[i]->eval()->tostring() << " = " << m_exps[i]->eval()->tostring() << endl;
            }
            return NULL;
        }

    private:
        std::vector<ASTNode*> m_vars;
        std::vector<ASTNode*> m_exps;
};






#endif
