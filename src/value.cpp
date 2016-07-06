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
void Enveronment::add(std::string name, Value* value) {
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
