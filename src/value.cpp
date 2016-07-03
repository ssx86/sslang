#include "value.h"
#include "node.h"
Value* FuncValue::call(Enveronment* env) {
    cout << "calling function!" << endl;
    BlockNode* blockNode = m_body->getBlock();
    if (blockNode->children_count() > 0){
        cout << "evaling block , size = " << m_body->children_count() << endl;
        return m_body->eval(env);
    } else {
        cout << "empty function!" << endl;
        return new StringValue("void");
    }

}
