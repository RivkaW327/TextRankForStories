
#include "IntervalTree.h"
#include <iostream>
#include <climits>
#include <memory>



Node::Node(size_t paragraphIndex, Interval i)
{
    this->paragraphIndex = paragraphIndex;
    this->i = i;
    this->max = i.high;
    this->left = nullptr;
    this->right = nullptr;
    this->height = 0;
}


// A utility function to 
// get the height of the tree 
int Node::Getheight() {
    if (this == nullptr)
        return 0;
    return this->height;
}

// A utility function to right 
// rotate subtree rooted with y 
std::unique_ptr<Node> Node::rightRotate(std::unique_ptr<Node> y) {
    std::unique_ptr<Node> x = std::move(y->left);
    if (x == nullptr)
        return y;

    std::unique_ptr<Node> T2 = std::move(x->right);

    x->right = std::move(y);
    x->right->left = std::move(T2);

    // Update heights and max
    x->right->updateHeightAndMax();
    x->updateHeightAndMax();

    return x;
}
// A utility function to left rotate 
// subtree rooted with x 
std::unique_ptr<Node> Node::leftRotate(std::unique_ptr<Node> x) {
    std::unique_ptr<Node> y = std::move(x->right);
    if (y == nullptr)
        return x;

    std::unique_ptr<Node> T2 = std::move(y->left);

    y->left = std::move(x);
    y->left->right = std::move(T2);

    // Update heights and max
    y->left->updateHeightAndMax();
    y->updateHeightAndMax();

    return y;
}

// Get balance factor of node N 
int Node::getBalance() {
    if (this->left == nullptr)
    {
        return 0 - this->right->Getheight();
    }
    else if (this->right == nullptr)
    {
        return this->left->Getheight();
    }
    else
    {
        return this->left->Getheight() - this->right->Getheight();
    }
}

void Node::updateHeightAndMax() {
    if (!this) return;
    this->height = 1 + std::max(this->left->Getheight(), this->right->Getheight());
    this->max = std::max(
        this->i.high,
        std::max(
            (this->left ? this->left->max : INT_MIN),
            (this->right ? this->right->max : INT_MIN)
        )
    );
}



std::unique_ptr<Node> Node::insertTree(std::unique_ptr<Node> root, std::unique_ptr<Node> n) {
    if (root == nullptr)
        return n;

    int n_low = n->i.low;

    if (n_low < root->i.low)
        root->left = insertTree(std::move(root->left), std::move(n));
    else
        root->right = insertTree(std::move(root->right), std::move(n));

    int balance = root->getBalance();

    // Left Left Case
    if (balance > 1 && n_low < root->left->i.low)
        return rightRotate(std::move(root));

    // Right Right Case
    if (balance < -1 && n_low > root->right->i.low)
        return leftRotate(std::move(root));

    // Left Right Case
    if (balance > 1 && n_low > root->left->i.low) {
        root->left = leftRotate(std::move(root->left));
        return rightRotate(std::move(root));
    }

    // Right Left Case
    if (balance < -1 && n_low < root->right->i.low) {
        root->right = rightRotate(std::move(root->right));
        return leftRotate(std::move(root));
    }

    root->updateHeightAndMax();

    return root;
}

bool Node::isOverlapping(Interval i1, Interval i2) {
    return i1.low <= i2.high && i2.low <= i1.high;
}

// לבדוק מה קורה אם הוא שייך ל2 פסקאות (לא שייך לשום פסקה בודדת)

Node* Node::overlapSearch(Interval i) {
    if (this == nullptr) return nullptr;

    if (isOverlapping(this->i, i))
        return this;

    if (this->left != nullptr && this->left->max >= i.low)
        return this->left->overlapSearch(i);

    return this->right->overlapSearch(i);
}

void Node::inorder() {
    if (this == nullptr) return;
    this->left->inorder();
    std::cout << "[" << this->i.low << ", " << this->i.high << "]"
        << " max = " << this->max << std::endl;
    this->right->inorder();
}

int Node::GetParagraphIndex() {
    return this->paragraphIndex;
}


