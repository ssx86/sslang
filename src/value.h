#ifndef _SS_VALUE_H_
#define _SS_VALUE_H_

#include <string>

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
