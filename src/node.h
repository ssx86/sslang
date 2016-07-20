#ifndef _SS_NODE_H_
#define _SS_NODE_H_

#include "value.h"
#include "token.h"

class ASTNode{
    public:
        virtual ~ASTNode();

        //children
        ASTNode* children(int i);
        int children_count();
        void addChild(ASTNode* child);

    public:
        std::vector<ASTNode*> m_children;

        virtual int type() = 0;
        virtual Value* eval(Enveronment* env) = 0;
        virtual std::string name() ;
};

class RetNode : public ASTNode {
    public:
        virtual int type()  { return 1; }
        virtual Value* eval(Enveronment* env);
};

class BlockNode : public ASTNode {
    public:
        virtual int type()  { return 2; }
        virtual Value* eval(Enveronment* env);
};

class NameNode : public ASTNode{
    public:
        virtual int type()  { return 3; }
        NameNode(Token* token);
        virtual Value* eval(Enveronment* env);
        std::string name();
    private:
    std::string m_name;
};


class LeafNode : public ASTNode {
    public:
        virtual int type()  { return 4; }
        LeafNode(); 
        LeafNode(std::string s);
        LeafNode(int i);
        LeafNode(double d); 
        LeafNode(bool b);
        virtual Value* eval(Enveronment* env);
    private:
        Value* m_value;
};

class FieldNode : public ASTNode {
    public:
        virtual int type()  { return 5; }
        FieldNode(); 
        void setKey(ASTNode* key);
        ASTNode* getKey();
        ASTNode* getValue();
        void setValue(ASTNode* value);
        virtual Value* eval(Enveronment* env);
    private:
        ASTNode* m_key;
        ASTNode* m_value; // ExpNode
};


class BinExpNode : public ASTNode {
    public:
        virtual int type()  { return 6; }
        BinExpNode(Token* token);
        void setLeft(ASTNode* left);
        void setRight(ASTNode* right);
        Token* getOp();
        ASTNode* getLeft();
        ASTNode* getRight();
        virtual Value* eval(Enveronment* env);
    private:
        Token* m_op;
        ASTNode* m_left;
        ASTNode* m_right;
};

class ForNode : public ASTNode {
    public:
        virtual int type()  { return 7; }
        ForNode();

        void setName(NameNode* name);
        void setFrom(ASTNode* exp);
        void setTo(ASTNode* exp);
        void setStep(ASTNode* exp);
        void setBlock(ASTNode* block);
        virtual Value* eval(Enveronment* env);
    private:
        NameNode* m_name;
        ASTNode* m_from;
        ASTNode* m_to;
        ASTNode* m_step;

        ASTNode* m_block;
};

class EmptyNode : public ASTNode {
    public:
        virtual int type()  { return 8; }
        virtual Value* eval(Enveronment* env);
};

class TableAccessNode : public ASTNode {
    public:
        virtual int type()  { return 9; }
        void setTable(ASTNode* table);
        void setIndex(ASTNode* index);
    public:
        virtual Value* eval(Enveronment* env);
        Value*& lvalue(Enveronment* env);

    private:
        ASTNode* m_table;
        ASTNode* m_index;
};

class AssignNode : public ASTNode {
    public:
        virtual int type()  { return 10; }
        void addVar(ASTNode* var);
        void addExp(ASTNode* exp);
        virtual Value* eval(Enveronment* env);

    private:
        std::vector<ASTNode*> m_vars;
        std::vector<ASTNode*> m_exps;
};


class FieldListNode : public ASTNode {
    public:
        virtual int type()  { return 11; }
        virtual Value* eval(Enveronment* env);
};

class NameListNode : public ASTNode {
    public:
        virtual int type()  { return 12; }
        virtual Value* eval(Enveronment* env);
};

class FuncBodyNode : public ASTNode {
    public:
        virtual int type()  { return 13; }
        FuncBodyNode();

        void setBlock(BlockNode* block);
        BlockNode* getBlock();
        void setParList(NameListNode* parlist);

        NameListNode* getParList();
        virtual Value* eval(Enveronment* env);
    private:
        BlockNode* m_block;
        NameListNode* m_parlist;
};

class FuncCallNode : public ASTNode {
    public:
        virtual int type()  { return 14; }
        void setArgs(ASTNode* args);
        void setName(ASTNode* name);
        virtual Value* eval(Enveronment* env);
    private:
        ASTNode* m_args;
        ASTNode* m_name;
};


class ExpListNode : public ASTNode {
    public:
        virtual int type()  { return 15; }
        virtual Value* eval(Enveronment* env);
};

class VarNode : public ASTNode {
    public:
        virtual int type()  { return 16; }
        virtual Value* eval(Enveronment* env);
};

class VarListNode : public ASTNode {
    public:
        virtual int type()  { return 17; }
        virtual Value* eval(Enveronment* env);
};


class LabelNode : public ASTNode {
    public:
        virtual int type()  { return 18; }
        virtual Value* eval(Enveronment* env);
};

class FunctionNode : public ASTNode {
    public:
        virtual int type()  { return 19; }
        void setName(ASTNode* name);
        void setBody(FuncBodyNode* body);
    public:
        virtual Value* eval(Enveronment* env);
    private:
        ASTNode* m_name;
        FuncBodyNode* m_body;
};

class CondNode {
    public:
        virtual int type()  { return 20; }
        void setCondition(ASTNode* cond);
        void setValue(ASTNode* value);
        bool getBool(Enveronment* env);
        Value* getValue(Enveronment* env);
    private:
        ASTNode* m_condition;
        ASTNode*  m_value;
};

class IfNode : public ASTNode {
    public:
        virtual int type()  { return 21; }
        std::vector<CondNode*> m_conds;
        void addCond(CondNode* cond);
        virtual Value* eval(Enveronment* env);
};

class RepeatNode : public ASTNode {
    public:
        virtual int type()  { return 22; }
        virtual Value* eval(Enveronment* env);
};

class WhileNode : public ASTNode {
    public:
        virtual int type()  { return 23; }
        virtual Value* eval(Enveronment* env);
};

class DoNode : public ASTNode {
    public:
        virtual int type()  { return 24; }
        virtual Value* eval(Enveronment* env);
};

class GotoNode : public ASTNode {
    public:
        virtual int type()  { return 25; }
        virtual Value* eval(Enveronment* env);
};

class BinOpHolderNode : public ASTNode {
    public:
        virtual int type()  { return 26; }
        BinOpHolderNode(Token* op);
        Token* token();
        int priority();
        virtual Value* eval(Enveronment* env);
    private:
        Token* m_op;
};

class FunctionDefNode : public ASTNode {
  public:
        virtual int type()  { return 27; }
    void setBody(FuncBodyNode* body);
  public:
    virtual Value* eval(Enveronment* env);
  private:
    FuncBodyNode* m_body;
};

class UnopExpNode : public ASTNode {
  public:
        virtual int type()  { return 28; }
    UnopExpNode(Token* op);
    void setExp(ASTNode* exp);
  private:
    ASTNode* m_exp;
    Token* m_op;

  public:
    virtual Value* eval(Enveronment* env);
};

class TableNode : public ASTNode {
    public:
        virtual int type()  { return 29; }
        void setField(FieldListNode* fieldlist);
    public:
        virtual Value* eval(Enveronment* env);
        
    private:
        FieldListNode* m_fieldlist;
};
       

#endif
