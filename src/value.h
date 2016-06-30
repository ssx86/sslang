#include <string>

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

