#ifndef _SS_VALUE_H_
#define _SS_VALUE_H_

#include <map>
#include <string>

using namespace std;

class Value;
class Enveronment {
    public:
    map<std::string, Value*> names;
    typedef map<std::string, Value*>::iterator iterator;
    Enveronment* next;
    void set(std::string name, Value* value) {
        names[name] = value;
    }
    Value* get(std::string name) {
        return names[name];
    }
};

class IntValue;
class DoubleValue;
class StringValue;
class BoolValue;

class Value {
    public:
        virtual std::string tostring() = 0;

        typedef enum {
            INT, 
            DOUBLE, 
            STRING, 
            BOOL,
            FUNC,
            DUMMY
        } Type;

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


        virtual Type type() = 0;


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
    private:
        std::string m_value;
};

class DoubleValue : public Value {
    public:
        DoubleValue(double d) {
            m_value = d;
        }
        double doubleValue() {
            return m_value;
        }
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%f", m_value);
            return std::string(temp);
        }
        virtual Type type() {
            return INT;
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
        Value* call(Enveronment* env);
        
    public:
        FuncBodyNode* m_body;
        std::string tostring() {
            return "function value";
        }
        virtual Type type() {
            return FUNC;
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
        std::string tostring() {
            char temp[100];
            sprintf(temp, "%d", m_value);
            return std::string(temp);
        }
        virtual Type type() {
            return INT;
        }
    private :
        int m_value;
};

#endif
