#include "node.h"
void ASTNode::addChild(ASTNode* child) {
    m_children.push_back(child);
}

ASTNode* ASTNode::children(int i) {
    return m_children[i];
}


