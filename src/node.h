#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "token.h"
#include "value.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;


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
class BlockNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            if (!env) 
                env = new Enveronment;

            Value* value = NULL;
            for(int i = 0; i < m_children.size(); i++) {
                
                value = children(i)->eval(env);
                if( dynamic_cast<RetNode*>( children(i) ) ) {
                    break;
                }
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

        ASTNode* getKey() {
            return m_key;
        }
        ASTNode* getValue() {
            return m_value;
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


class BinExpNode : public ASTNode {
    public:
        BinExpNode(Token* token) : m_left(NULL), m_right(NULL) {
            m_op = token;
        }
        void setLeft(ASTNode* left) {
            m_left = left;
        }
        void setRight(ASTNode* right) {
            m_right = right;
        }
        Token* getOp() {
            return m_op;
        }
        ASTNode* getLeft() {
            return m_left;
        }
        ASTNode* getRight() {
            return m_right;
        }

        virtual Value* eval(Enveronment* env) {
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
                    if (! env->update(varName, value)) {  //先检索祖先环境链。这是和local赋值的唯一区别
                        env->add(varName, value);
                    }
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
            return NULL; // never eval
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
            if ( m_name == "print" ) {
                Value* value = m_args->children(0)->eval(env);
                cout << value->tostring() << endl;
                return value;
            } else {
                //cout << "find a function call, trying to run it: " << m_name << endl;
                FuncValue* funcValue = dynamic_cast<FuncValue*>(env->get(m_name));

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
                    cout << "error" << endl;
                    exit(1);
                    return NULL;
                    
                }
            }
        }
    private:
        ASTNode* m_args;
        std::string m_name;
};


class ExpListNode : public ASTNode {
    public:
        virtual Value* eval(Enveronment* env) {
            Value* value = NULL;
            for(int i = 0; i < m_children.size(); i++) {
                value = children(i)->eval(env);
            }
            assert(value);
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
            env->add(m_name->name(), new FuncValue(m_body));
            //cout << "adding a function named: " << "<" << m_name->name() << ">" << endl;
            //cout << " with block size = " << "<" << m_body->getBlock()->children_count() << ">" << endl;
              
              
            return new StringValue("<FunctionNode>");
        }
    private:
        FuncNameNode* m_name;
        FuncBodyNode* m_body;
};

class CondNode {
    public:
        void setCondition(ASTNode* cond) {
            m_condition = cond;
        }
        void setValue(ASTNode* value) {
            m_value = value;
        }
        bool getBool(Enveronment* env) {
            if(!m_condition)
                return true;

            return m_condition->eval(env)->boolValue();
        }
        Value* getValue(Enveronment* env) {
            return m_value->eval(env);
        }
    private:
        ASTNode* m_condition;
        ASTNode*  m_value;
};
class IfNode : public ASTNode {
    public:
        vector<CondNode*> m_conds;
        void addCond(CondNode* cond) {
            m_conds.push_back(cond);
        }
        virtual Value* eval(Enveronment* env) {
            for(int i = 0; i < m_conds.size(); i++) {
                if( m_conds[i]->getBool(env) )
                    return m_conds[i]->getValue(env);
            }
            Value* value = new StringValue("<IfNode>");
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

class BinOpHolderNode : public ASTNode {
    public:
        BinOpHolderNode(Token* op) {
            m_op = op;
        }
        Token* token() {
            return m_op;
        }
        int priority() {
            return m_op->pri();
        }
        virtual Value* eval(Enveronment* env) {
            return NULL; // never eval
        }
    private:
        Token* m_op;
};

class FunctionDefNode : public ASTNode {
  public:
    void setBody(FuncBodyNode* body) {
      m_body = body;
    }
  public:
    virtual Value* eval(Enveronment* env) {
      return new FuncValue(m_body);
    }
  private:
    FuncBodyNode* m_body;
};

class UnopExpNode : public ASTNode {
  public:
    UnopExpNode(Token* op) {
      m_op = op;
    }
    void setExp(ASTNode* exp) {
      m_exp = exp;
    }
  private:
    ASTNode* m_exp;
    Token* m_op;

  public:
    virtual Value* eval(Enveronment* env) {
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
          return m_exp->eval(env);
        case Token::NOT:
            return new BoolValue( ! m_exp->eval(env)->boolValue() );
        default:
            exit(1);
      }
    }
};

class TableNode : public ASTNode {
    public:
        void setField(FieldListNode* fieldlist) {
            m_fieldlist = fieldlist;
        }

    public:
        virtual Value* eval(Enveronment* env) {
            TableValue* table = new TableValue();
            int pos = 0;
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
    private:
        FieldListNode* m_fieldlist;
};
       

#endif
