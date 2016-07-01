#include "value.h"
#include "node.h"
Value* FuncValue::call(Enveronment* env) {
    return m_body->eval(env);
}
