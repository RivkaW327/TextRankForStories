#include "IntervalTree.h"
#include <iostream>

Node* newNode(size_t pi, Interval i) {
    Node* temp = new Node;
    temp->paragraphIndex = pi;
    temp->i = new Interval(i);
    temp->max = i.high;
    temp->left = temp->right = nullptr;
    return temp;
}

//לעדכן למאוזן!!!!!!!!!!
Node* insertTree(Node* root, Node* n) {
    if (root == nullptr)
        return n;

    int l = n->i->low;

    if (n->i->low < l)
        root->left = insertTree(root->left, n);
    else
        root->right = insertTree(root->right, n);

    if (root->max < n->i->high)
        root->max = n->i->high;

    return root;
}

bool isOverlapping(Interval i1, Interval i2) {
    return i1.low <= i2.high && i2.low <= i1.high;
}

Node* overlapSearch(Node* root, Interval i) {
    if (root == nullptr) return nullptr;

    if (isOverlapping(*(root->i), i))
        return root;

    if (root->left != nullptr && root->left->max >= i.low)
        return overlapSearch(root->left, i);

    return overlapSearch(root->right, i);
}

void inorder(Node* root) {
    if (root == nullptr) return;
    inorder(root->left);
    std::cout << "[" << root->i->low << ", " << root->i->high << "]"
        << " max = " << root->max << std::endl;
    inorder(root->right);
}
