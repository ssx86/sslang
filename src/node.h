#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include "value.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

Value* execute(Token* op, Value* left, Value* right, Value::Type retType);


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
        virtual std::string name() {
            return "ASTNode Name UNKNOWN";
        }
};

class BlockNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            if (!env) 
                env = new Enveronment;

            Value* value = NULL;
            for(int i = 0; i < m_children.size(); i++) {
                value = children(i)->eval(env);
                std::cout << value->tostring() << std::endl;
            }
            assert(value);
            return value;
        }
};


class NameNode : public ASTNode{
    public:
        NameNode(Token* token) {
            m_name = token->tostring();
        }
        virtual Value* eval(Enveronment* env) {
            return env->get(m_name);
        }
        std::string name() {
            return m_name;
        }
    private:
    std::string m_name;
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
        LeafNode() { //TODO value nil!!!
            m_value = new StringValue("nil");
        }
        LeafNode(std::string &s) {
            m_value = new StringValue(s);
        }
        LeafNode(int i) {
            m_value = new IntValue(i);
        }
        LeafNode(double d) {
            m_value = new DoubleValue(d);
        }
        LeafNode(bool b) {
            m_value = new BoolValue(b);
        }
        virtual Value* eval(Enveronment* env) {
            return m_value;
        }
    private:
        Value* m_value;
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
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_children.size(); i++) {
                std::cout << children(i)->eval(env)->tostring() << std::endl;
            }
            return new StringValue("<FieldNode>");
        }
    private:
        ASTNode* m_key;
        ASTNode* m_value; // ExpNode
};


class BinopNode : public ASTNode {
    public:
        BinopNode(Token* token) : m_left(NULL), m_right(NULL) {
            m_op = token;
        }
        void setLeft(ASTNode* left) {
            m_left = left;
        }
        void setRight(ASTNode* right) {
            m_right = right;
        }

        virtual Value* eval(Enveronment* env) {
            Value* leftValue = m_left->eval(env);
            Value* rightValue = m_right->eval(env);

            if(Value::INT == leftValue->type() && Value::INT == rightValue->type() ) {
                return execute(m_op, leftValue, rightValue, Value::INT);
            } else if (Value::DOUBLE == leftValue->type() && 
                    (Value::INT == rightValue->type() || Value::DOUBLE == rightValue->type())) {
                return execute(m_op, leftValue, rightValue, Value::DOUBLE);
            } else if (Value::DOUBLE == rightValue->type() && 
                    (Value::INT == leftValue->type() || Value::DOUBLE == leftValue->type())) {
                return execute(m_op, leftValue, rightValue, Value::DOUBLE);
            } else if (Value::DOUBLE == leftValue->type() && Value::DOUBLE == rightValue->type()) {
                return execute(m_op, leftValue, rightValue, Value::DOUBLE);
            } else if (Value::STRING == leftValue->type() && Value::STRING == rightValue->type()) {
                return execute(m_op, leftValue, rightValue, Value::STRING);
            } else return new IntValue(100);
        }
    private:
        Token* m_op;
        ASTNode* m_left;
        ASTNode* m_right;
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

            Value* value = NULL;
            int from = m_from->eval(env)->intValue();
            int to = m_to->eval(env)->intValue();

            env->set(m_name->name(), new IntValue(from));


            while( env->get(m_name->name())->intValue() <= to ) {
                std::cout << "loop(" << from << ")" << std::endl;
                int step = 1;
                if(m_step) {
                    step = m_step->eval(env)->intValue();
                }

                from += step;
                to = m_to->eval(env)->intValue();

                value = m_block->eval(env);

                env->set(m_name->name(), new IntValue(from));
            }
            assert(value);
            return value;
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
                NameNode* pName = dynamic_cast<NameNode*>(m_vars[i]);
                if(pName) {
                    std::string varName = pName->name();
                    value = m_exps[i]->eval(env);
                    env->names[varName] = value;
                    std::cout << "Assign: " << varName << " := " << value->tostring() << std::endl;
                } else {
                    std::cout << "error reading var name" << std::endl;
                }

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
class FuncBodyNode : public ASTNode {
    public:
        FuncBodyNode() : m_block(NULL), m_parlist(NULL) {}

        void setBlock(BlockNode* block) {
            m_block = block;
        }
        BlockNode* getBlock() {
            return m_block;
        }
        void setParList(NameListNode* parlist) {
            m_parlist = parlist;
        }

        NameListNode* getParList() {
            return m_parlist;
        }
        virtual Value* eval(Enveronment* env) {
            return m_block->eval(env);
        }
    private:
        BlockNode* m_block;
        NameListNode* m_parlist;
};
class FuncCallNode : public ASTNode {
    public:
        void setArgs(ASTNode* args) {
            m_args = args;
        }
        void setName(ASTNode* name) {
            m_name = name->name();
        }
        void appendName(ASTNode* name) {
            m_name += "::";
            m_name += name->name();
        }
        std::string name() {
            return m_name;
        }
        virtual Value* eval(Enveronment* env) {
            cout << "find a function call, trying to run it: " << m_name << endl;
            FuncValue* funcValue = dynamic_cast<FuncValue*>(env->get(m_name));

            cout << "#############=> " << funcValue->getBodyNode()->getBlock()->children_count() << endl;

            if (funcValue) {
                NameListNode* parList = funcValue->getBodyNode()->getParList();

                    
                // 设置参数环境
                Enveronment *localEnv = new Enveronment;
                localEnv->next = env;

                for(int i = 0; i < parList->children_count(); i++) {
                    cout << "par: " << parList->children(i)->name() << endl;
                    std::string name = parList->children(i)->name();
                    localEnv->set(name, m_args->children(i)->eval(env));
                }

                return funcValue->call(localEnv);
            } else {
                Value* value = new StringValue("<FuncCallNode>");
                return value;
            }
        }
    private:
        ASTNode* m_args;
        std::string m_name;
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
        FuncNameNode() {
            m_name = "";
        }
        void setName(NameNode* name) {
            m_name += name->name();
        }
        void appendName(NameNode* name) {
            m_name += "::";
            m_name += name->name();
        }
        std::string name() {
            return m_name;
        }
        virtual Value* eval(Enveronment* env) {
            Value* value = new StringValue("<FuncNameNode>");
            return value;
        }
    public:
        std::string m_name;
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
            Value* value = NULL;
            for(int i = 0; i < m_children.size(); i++) {
                value = children(i)->eval(env);
            }
            return value;
        }
};
class FunctionNode : public ASTNode {
    public:
        void setName(FuncNameNode* name) {
            m_name = name;
        }

        void setBody(FuncBodyNode* body) {
            m_body = body;
        }
    public:
        virtual Value* eval(Enveronment* env) {
            env->set(m_name->name(), new FuncValue(m_body));
            cout << "adding a function named: " << "<" << m_name->name() << ">" << endl;
            cout << " with block size = " << "<" << m_body->getBlock()->children_count() << ">" << endl;
              
              
            return new StringValue("<FunctionNode>");
        }
    private:
        FuncNameNode* m_name;
        FuncBodyNode* m_body;
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
