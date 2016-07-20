#include "value.h"
#include "node.h"

#include <iostream>
#include <cassert>

using namespace std;

/*********************************
 * Enveronment
 * *******************************/

Enveronment::Enveronment() : next(NULL) {

}

Enveronment* Enveronment::merge(Enveronment* env) {
    Enveronment* newEnv = new Enveronment;
    Enveronment* p = env;
    while(p) {

        for(IterType it = p->names.begin(); it != p->names.end(); it++) {
            if( newEnv->names.find(it->first) == newEnv->names.end() ) { // not found
                newEnv->names[it->first] = it->second;
            }
        }
        p = p->next;
    }
    return newEnv;
}

Value* Enveronment::get(std::string name) {
    IterType it = names.find(name);
    if(it == names.end()) 
    {
        if(next == NULL) {
            return new NilValue;
        } else {
            return next->get(name);
        }
    } else {
        return it->second;
    }
}

void Enveronment::add(std::string name, Value* value) {
    //std::cout << this << " , adding: " << name << " , value: " << value->tostring() << std::endl;
    names[name] = value;
}

bool Enveronment::update(std::string name, Value* value) {
    IterType it = names.find(name);
    if(it == names.end()) 
    {
        if(next == NULL) {
            return false;
        } else {
            return next->update(name, value);
        }
    } else {
        it->second = value;
        return true;
    }
}


/*********************************
 * Value
 * *******************************/

std::string Value::stringValue() {
    return "";
}
bool Value::boolValue() {
    return true;
}
double Value::doubleValue() {
    return 0.0;
}
int Value::intValue() {
    return 0;
}

/*********************************
 * StringValue
 * *******************************/

StringValue::StringValue(const char* str) {
    m_value = str;
}

StringValue::StringValue(std::string str) {
    m_value = str;
}
std::string StringValue::stringValue() {
    return m_value;
}

std::string StringValue::tostring() {
    return m_value;
}
Value::Type StringValue::type() {
    return STRING;
}
bool StringValue::eq(Value* other) {
    return type() == other->type() && m_value == other->stringValue();
}

/*********************************
 * TableValue
 * *******************************/

std::string TableValue::tostring() {
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
bool TableValue::eq(Value* other) {
    return this == other;
}

Value::Type TableValue::type() {
    return TABLE;
}

std::string TableValue::stringValue() {
    return "";
}
bool TableValue::boolValue() {
    return true;
}
double TableValue::doubleValue() {
    return 0.0;
}
int TableValue::intValue() {
    return 0;
}

int TableValue::getLength() {
    return m_array.size();
}

Value*& TableValue::getArraylValue(int index) {
    assert(index > 0);
    return m_array[index - 1];
}

Value*& TableValue::getMaplValue(std::string name) {
    return m_map[name];
}
Value* TableValue::getArrayValue(int pos) {
    assert(pos > 0);
    return m_array[pos - 1];
}
Value* TableValue::getMapValue(std::string name) {
    return m_map[name];
}

void TableValue::setArrayValue(int pos, Value* value) {
    assert(pos > 0);
    if(pos-1 >= m_array.size() )
        m_array.resize(pos);
    m_array[pos-1] = value;
}
void TableValue::setMapValue(std::string name, Value* value) {
    m_map[name] = value;
}

/*********************************
 * DoubleValue
 * *******************************/

DoubleValue::DoubleValue(double d) {
    m_value = d;
}
double DoubleValue::doubleValue() {
    return m_value;
}
bool DoubleValue::boolValue() {
    return m_value != 0;
}
std::string DoubleValue::tostring() {
    char temp[100];
    sprintf(temp, "%.13f", m_value);
    return std::string(temp);
}
Value::Type DoubleValue::type() {
    return INT;
}
bool DoubleValue::eq(Value* other) {
    return type() == other->type() && m_value == other->doubleValue();
}

/*********************************
 * BoolValue
 * *******************************/

BoolValue::BoolValue(bool b) {
    m_value = b;
}
bool BoolValue::boolValue() {
    return m_value;
}
std::string BoolValue::tostring() {
    if(m_value) 
        return "true";
    else
        return "false";
}
Value::Type BoolValue::type() {
    return BOOL;
}
bool BoolValue::eq(Value* other) {
    return m_value == other->boolValue();
}

/*********************************
 * NilValue
 * *******************************/

std::string NilValue::tostring() {
    return "nil";
}

bool NilValue::eq(Value* other) {
    return dynamic_cast<NilValue*>(other);
}

Value::Type NilValue::type() {
    return NIL;
}

std::string NilValue::stringValue() {
    return "nil";
}

bool NilValue::boolValue() {
    return false;
}

double NilValue::doubleValue() {
    return 0.0;
}

int NilValue::intValue() {
    return 0;
}

/*********************************
 * FuncValue
 * *******************************/

Value* FuncValue::call(Enveronment* env) {
    BlockNode* blockNode = m_body->getBlock();
    if (blockNode->children_count() > 0){
        if (m_env)
        {
            //插到第二位，跳过参数。参数是优先级最高的
            Enveronment* temp = env->next;
            env->next = m_env;
            m_env->next = temp;
        }
        return blockNode->eval(env);
    } else {
        return new StringValue("void");
    }

}
FuncValue::FuncValue(FuncBodyNode* body, Enveronment* env) {
    m_body = body;
    if(env) {
        m_env = Enveronment::merge(env);
    } else
        m_env = NULL;
}
FuncBodyNode* FuncValue::getBodyNode() {
    return m_body;
}
Value* FuncValue::call(Enveronment* env);

std::string FuncValue::tostring() {
    return "function value";
}

Value::Type FuncValue::type() {
    return FUNC;
}

bool FuncValue::eq(Value* other) {
    return other == this; // address equal
}

/*********************************
 * IntValue
 * *******************************/

IntValue::IntValue(int i) {
    m_value = i;
}
int IntValue::intValue() { 
    return m_value;
}
double IntValue::doubleValue() {
    return m_value;
}
bool IntValue::boolValue() {
    return m_value != 0;
}
std::string IntValue::tostring() {
    char temp[100];
    sprintf(temp, "%d", m_value);
    return std::string(temp);
}

Value::Type IntValue::type() {
    return INT;
}

bool IntValue::eq(Value* other) {
    return type() == other->type() && m_value == other->intValue();
}

/*********************************
 * BinExpEval
 * *******************************/

// add
Value* BinExpEval::AddOp(Value* left, Value* right) {
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
Value* BinExpEval::SubOp(Value* left, Value* right) {
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
Value* BinExpEval::MulOp(Value* left, Value* right) {
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
Value* BinExpEval::DivOp(Value* left, Value* right) {
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
Value* BinExpEval::AndOp(Value* left, Value* right) {
    return new BoolValue(left->boolValue() && right->boolValue());
}

// or
Value* BinExpEval::OrOp(Value* left, Value* right) {
    return new BoolValue(left->boolValue() || right->boolValue());
}

// eq
Value* BinExpEval::EqOp(Value* left, Value* right) {
    return new BoolValue(left->eq(right));
}

// gt
Value* BinExpEval::GTOp(Value* left, Value* right) {
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
Value* BinExpEval::GEOp(Value* left, Value* right) {
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
Value* BinExpEval::LTOp(Value* left, Value* right) {
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
Value* BinExpEval::LEOp(Value* left, Value* right) {
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
Value* BinExpEval::NEOp(Value* left, Value* right) {
    return new BoolValue(! left->eq(right));
}

Value* BinExpEval::ConcatOp(Value* left, Value* right) {
    return new StringValue(left->tostring() + right->tostring());
}


