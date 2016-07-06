#ifndef _SS_VALUE_H_
#define _SS_VALUE_H_

#include <map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Value;
class Enveronment {
    public:
        Enveronment() : next(NULL) {}

        map<std::string, Value*> names;
        typedef map<std::string, Value*>::iterator IterType;
        Enveronment* next;

        void add(std::string name, Value* value);
        bool update(std::string name, Value* value);

        Value* find(std::string name) {
            return get(name, false);
        }

        Value* get(std::string name, bool force = true) {
            IterType it = names.find(name);
            if(it == names.end()) 
            {
                if(next == NULL) {
                    if(force) {
                        cerr << "undefined name: " << name << endl;
                        exit(0);
                    } else {
                        return NULL;
                    }
                } else {
                    return next->get(name);
                }
            }
            return it->second;
        }
};

class IntValue;
class DoubleValue;
class StringValue;
class BoolValue;
class TableValue;

class Value {
    public:
        typedef enum {
            INT, 
            DOUBLE, 
            STRING, 
            BOOL,
            FUNC,
            TABLE,
            DUMMY
        } Type;

        virtual std::string tostring() = 0;
        virtual bool eq(Value* other) = 0;
        virtual Type type() = 0;

        virtual std::string stringValue() {
            return "";
        }
        virtual bool boolValue() {
            return true;
        }
        virtual double doubleValue() {
            return 0.0;
        }
        virtual int intValue() {
            return 0;
        }
};

class StringValue : public Value {
    public:
        StringValue(const char* str) {
            m_value = str;
        }
        StringValue(std::string str) {
            m_value = str;
        }
        std::string stringValue() {
            return m_value;
        }

        std::string tostring() {
            return m_value;
        }
        virtual Type type() {
            return STRING;
        }
        virtual bool eq(Value* other) {
            return type() == other->type() && m_value == other->stringValue();
        }
    private:
        std::string m_value;
};

class TableValue : public Value {
    public:
        virtual std::string tostring() {
            std::string str = "{";
            for(int i = 0; i < m_array.size(); i++) {
                str += m_array[i]->tostring();
                if (i < m_array.size() - 1) {
                    str += ", ";
                }
            }
            std::map<std::string, Value*>::iterator it; 
            for(it = m_map.begin(); it != m_map.end(); it++) {
                str += ", ";
                str += it->first;
                str += "=";
                str += it->second->tostring();
            }
            str += "}";
            return str;
        }
        virtual bool eq(Value* other) {
            return this == other;
        }

        virtual Type type() {
            return TABLE;
        }

        virtual std::string stringValue() {
            return "";
        }
        virtual bool boolValue() {
            return true;
        }
        virtual double doubleValue() {
            return 0.0;
        }
        virtual int intValue() {
            return 0;
        }

        void setArrayValue(int pos, Value* value) {
            if(pos >= m_array.size() )
                m_array.resize(pos+1);
            m_array[pos] = value;
        }
        void setMapValue(std::string name, Value* value) {
            m_map[name] = value;
        }
    private:
        std::vector<Value*> m_array;
        std::map<std::string, Value*> m_map;
};

class DoubleValue : public Value {
    public:
        DoubleValue(double d) {
            m_value = d;
        }
        double doubleValue() {
            return m_value;
        }
        virtual bool boolValue() {
            return m_value != 0;
        }
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%.13f", m_value);
            return std::string(temp);
        }
        virtual Type type() {
            return INT;
        }
        virtual bool eq(Value* other) {
            return type() == other->type() && m_value == other->doubleValue();
        }
    private :
        double m_value;
};

class BoolValue : public Value {
    public:
        BoolValue(bool b) {
            m_value = b;
        }
        bool boolValue() {
            return m_value;
        }
        std::string tostring() {
            if(m_value) 
                return "true";
            else
                return "false";
        }
        virtual Type type() {
            return BOOL;
        }
        virtual bool eq(Value* other) {
            return m_value == other->boolValue();
        }
    private:
        bool m_value;
};

class NilValue : public Value {
};

class FuncBodyNode;
class FuncValue : public Value {
    public:
        FuncValue(FuncBodyNode* body) {
            m_body = body;
        }
        FuncBodyNode* getBodyNode() {
            return m_body;
        }
        Value* call(Enveronment* env);

    public:
        FuncBodyNode* m_body;
        std::string tostring() {
            return "function value";
        }
        virtual Type type() {
            return FUNC;
        }
        virtual bool eq(Value* other) {
            return other == this; // address equal
        }
};

class IntValue : public Value {
    public:
        IntValue(int i) {
            m_value = i;
        }
        int intValue() { 
            return m_value;
        }
        double doubleValue() {
            return m_value;
        }
        virtual bool boolValue() {
            return m_value != 0;
        }
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%d", m_value);
            return std::string(temp);
        }
        virtual Type type() {
            return INT;
        }
        virtual bool eq(Value* other) {
            return type() == other->type() && m_value == other->intValue();
        }
    private :
        int m_value;
};

class BinExpEval {
    public:

    // add
    static Value* AddOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new IntValue(left->intValue() + right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new DoubleValue(left->doubleValue() + right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->intValue() + right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->doubleValue() + right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }

    }

    // sub
    static Value* SubOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new IntValue(left->intValue() - right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new DoubleValue(left->doubleValue() - right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->intValue() - right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->doubleValue() - right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // mul
    static Value* MulOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new IntValue(left->intValue() * right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new DoubleValue(left->doubleValue() * right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->intValue() * right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->doubleValue() * right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }
   
    // div
    static Value* DivOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            if(left->intValue() % right->intValue() == 0)
                return new IntValue(left->intValue() / right->intValue());
            else
                return new DoubleValue(left->doubleValue() / right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new DoubleValue(left->doubleValue() / right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->intValue() / right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new DoubleValue(left->doubleValue() / right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // and
    static Value* AndOp(Value* left, Value* right) {
        return new BoolValue(left->boolValue() && right->boolValue());
    }

    // or
    static Value* OrOp(Value* left, Value* right) {
        return new BoolValue(left->boolValue() || right->boolValue());
    }

    // eq
    static Value* EqOp(Value* left, Value* right) {
        return new BoolValue(left->eq(right));
    }

    // gt
    static Value* GTOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->intValue() > right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->doubleValue() > right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->intValue() > right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->doubleValue() > right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // ge
    static Value* GEOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->intValue() >= right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->doubleValue() >= right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->intValue() >= right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->doubleValue() >= right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // lt
    static Value* LTOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->intValue() < right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->doubleValue() < right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->intValue() < right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->doubleValue() < right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // le
    static Value* LEOp(Value* left, Value* right) {
        if(dynamic_cast<IntValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->intValue() <= right->intValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<IntValue*>(right)) {
            return new BoolValue(left->doubleValue() <= right->intValue());
        } else if(dynamic_cast<IntValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->intValue() <= right->doubleValue());
        } else if(dynamic_cast<DoubleValue*>(left) && dynamic_cast<DoubleValue*>(right)) {
            return new BoolValue(left->doubleValue() <= right->doubleValue());
        } else {
            return new StringValue("Error Bin op");
        }
    }

    // ne
    static Value* NEOp(Value* left, Value* right) {
        return new BoolValue(! left->eq(right));
    }

    static Value* ConcatOp(Value* left, Value* right) {
        return new StringValue(left->tostring() + right->tostring());
    }

};


#endif
