#include "value.h"
#include "node.h"
Value* FuncValue::call(Enveronment* env) {
    BlockNode* blockNode = m_body->getBlock();
    if (blockNode->children_count() > 0){
        return blockNode->eval(env);
    } else {
        return new StringValue("void");
    }

}
void Enveronment::set(std::string name, Value* value) {
    names[name] = value;
}
