#ifndef _SS_VALUE_H_
#define _SS_VALUE_H_

#include <vector>
#include <string>
#include <map>

class Value;

class Enveronment {
    public:
        Enveronment();

        std::map<std::string, Value*> names;
        typedef std::map<std::string, Value*>::iterator IterType;
        Enveronment* next;

        void add(std::string name, Value* value);
        bool update(std::string name, Value* value);

        Value* get(std::string name);
};

class Value {
    public:
        typedef enum {
            INT, 
            DOUBLE, 
            STRING, 
            BOOL,
            FUNC,
            TABLE,
            NIL,
            DUMMY
        } Type;

        virtual std::string tostring() = 0;
        virtual bool eq(Value* other) = 0;
        virtual Type type() = 0;

        virtual std::string stringValue();
        virtual bool boolValue();
        virtual double doubleValue();
        virtual int intValue();
};

class StringValue : public Value {
    public:
        StringValue(const char* str);
        StringValue(std::string str);
        std::string stringValue();

        std::string tostring();
        virtual Type type();
        virtual bool eq(Value* other);
    private:
        std::string m_value;
};

class TableValue : public Value {
    public:
        virtual std::string tostring();
        virtual bool eq(Value* other);
        virtual Type type();

        virtual std::string stringValue();
        virtual bool boolValue();
        virtual double doubleValue();
        virtual int intValue();

        int getLength();

        Value*& getArraylValue(int index);
        Value*& getMaplValue(std::string name);
        Value* getArrayValue(int pos);
        Value* getMapValue(std::string name);

        void setArrayValue(int pos, Value* value);
        void setMapValue(std::string name, Value* value);
    private:
        std::vector<Value*> m_array;
        std::map<std::string, Value*> m_map;
};

class DoubleValue : public Value {
    public:
        DoubleValue(double d);
        double doubleValue();
        virtual bool boolValue();
        std::string tostring();
        virtual Type type();
        virtual bool eq(Value* other);
    private :
        double m_value;
};

class BoolValue : public Value {
    public:
        BoolValue(bool b);
        bool boolValue();
        std::string tostring();
        virtual Type type();
        virtual bool eq(Value* other);
    private:
        bool m_value;
};

class NilValue : public Value {
    public:
        virtual std::string tostring();
        virtual bool eq(Value* other);
        virtual Type type();

        virtual std::string stringValue();
        virtual bool boolValue();
        virtual double doubleValue();
        virtual int intValue();
};

class FuncBodyNode;
class FuncValue : public Value {
    public:
        FuncValue(FuncBodyNode* body);
        FuncBodyNode* getBodyNode();
        Value* call(Enveronment* env);

    public:
        FuncBodyNode* m_body;
        std::string tostring();
        virtual Type type();
        virtual bool eq(Value* other);
};

class IntValue : public Value {
    public:
        IntValue(int i);
        int intValue();
        double doubleValue();
        virtual bool boolValue();
        std::string tostring();
        virtual Type type();
        virtual bool eq(Value* other);
    private :
        int m_value;
};

class BinExpEval {
    public:

    // add
    static Value* AddOp(Value* left, Value* right);

    // sub
    static Value* SubOp(Value* left, Value* right);

    // mul
    static Value* MulOp(Value* left, Value* right);
   
    // div
    static Value* DivOp(Value* left, Value* right);

    // and
    static Value* AndOp(Value* left, Value* right);

    // or
    static Value* OrOp(Value* left, Value* right);

    // eq
    static Value* EqOp(Value* left, Value* right);

    // gt
    static Value* GTOp(Value* left, Value* right);

    // ge
    static Value* GEOp(Value* left, Value* right);

    // lt
    static Value* LTOp(Value* left, Value* right);

    // le
    static Value* LEOp(Value* left, Value* right);

    // ne
    static Value* NEOp(Value* left, Value* right);

    static Value* ConcatOp(Value* left, Value* right);

};


#endif
