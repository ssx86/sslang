#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include "value.h"
#include <iostream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

    

class Enveronment {
    public:
    map<std::string, Value*> value;
    Enveronment* next;
};

class ASTNode{
    public:
        virtual ~ASTNode() {}

        //children
        ASTNode* children(int i);
        int children_count() {
            return m_children.size();
        }
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;

        virtual Value* eval(Enveronment* env) = 0;
};

class ExpNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            Value* value = new StringValue("<ExpNode>");
            return value;
        }
};
class BlockNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            if (!env) 
                env = new Enveronment;

            Value* value;
            for(int i = 0; i < m_children.size(); i++) {
                value = children(i)->eval(env);
                std::cout << value->tostring() << std::endl;
            }
            return value;
        }
};


class NameNode : public ASTNode{
    public:
        NameNode(Token* token) {
            m_token = token;
        }
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
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
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            return new StringValue(m_op->tostring());
        }

    private:
        Token* m_op;
};

class LeafNode : public ASTNode {
    public:
        LeafNode(Token* token) {
            m_token = token;
        }
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
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
        void setValue(ExpNode* value) {
            m_value = value;
        }
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            return new StringValue("<FieldNode>");
        }
    private:
        ASTNode* m_key;
        ExpNode* m_value; // ExpNode
};

class BinopNode : public ASTNode {
    public:
        BinopNode(Token* token) {
            m_op = token;
        }

        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
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
        virtual Value* eval(Enveronment* env) {
            m_from->eval(env);
            m_to->eval(env);
            if(m_step) {
                m_step->eval(env);
            }
            return m_block->eval(env);
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
        virtual Value* eval(Enveronment* env) {
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
        virtual Value* eval(Enveronment* env) {

            Value* value = NULL;
            for(int i = 0; i < (int)m_vars.size(); i++) {
                value = m_exps[i]->eval(env);
                env->value[m_vars[i]->eval(env)->tostring()] = value;
            }
            return value;
        }

    private:
        std::vector<ASTNode*> m_vars;
        std::vector<ASTNode*> m_exps;
};


class FieldListNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<FieldListNode>");
            return value;
        }
};
class FunctionBodyNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<FunctionBodyNode>");
            return value;
        }
};
class FunctionCallNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<FunctionCallNode>");
            return value;
        }
};


class ExpListNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<ExpListNode>");
            return value;
        }
};
class NameListNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<NameListNode>");
            return value;
        }
};
class VarNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<VarNode>");
            return value;
        }
};
class VarListNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<VarListNode>");
            return value;
        }
};
class FuncNameNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<FuncNameNode>");
            return value;
        }
};
class LabelNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<LabelNode>");
            return value;
        }
};
class RetNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<RetNode>");
            return value;
        }
};
class FunctionNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<FunctionNode>");
            return value;
        }
};
class IfNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<IfNode>");
            return value;
        }
};
class CondNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<CondNode>");
            return value;
        }
};
class RepeatNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<RepeatNode>");
            return value;
        }
};
class WhileNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<WhileNode>");
            return value;
        }
};

class DoNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<DoNode>");
            return value;
        }
};


class GotoNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            Value* value = new StringValue("<GotoNode>");
            return value;
        }
};



#endif
