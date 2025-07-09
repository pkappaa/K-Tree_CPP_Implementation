#include "KDNode.hpp"

KDNode::KDNode(Embedding& e, int split_axis, KDNode* left, KDNode* right, KDNode* parent)
    : e(e), split_axis(split_axis), left(left), right(right), parent(parent)
{
}
 
KDNode::KDNode(const KDNode& node)
    : e(node.e), split_axis(node.split_axis), left(nullptr), right(nullptr), parent(nullptr)
{
    if (node.left) {
        left = new KDNode(*node.left);
        left->parent = this;
    }
    if (node.right) {
        right = new KDNode(*node.right);
        right->parent = this;
    }
}

KDNode::~KDNode() {
    delete left;
    delete right;
}

Embedding& KDNode::getEmbedding() const{
    return const_cast<Embedding&>(e);
}

KDNode* KDNode::getLeft() const {
    return left;
}

KDNode* KDNode::getRight() const {
    return right;
}

KDNode* KDNode::getParent() const {
    return parent;
}

void KDNode::setLeft(KDNode* node) {
    left = node;
}

void KDNode::setRight(KDNode* node) {
    right = node;
}

void KDNode::setParent(KDNode* node) {
    parent = node;
}
