#include "IntervalTree.h"
#include <iostream>
#include <climits>


// A utility function to 
// get the height of the tree 
int height(Node* N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

// A utility function to right 
// rotate subtree rooted with y 
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation 
    x->right = y;
    y->left = T2;

    //// Update heights 
    //y->height = 1 + std::max(height(y->left),
    //    height(y->right));
    //x->height = 1 + std::max(height(x->left),
    //    height(x->right));

    // Update heights and max
    updateHeightAndMax(y);
    updateHeightAndMax(x);


    // Return new root 
    return x;
}

// A utility function to left rotate 
// subtree rooted with x 
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation 
    y->left = x;
    x->right = T2;

    //// Update heights 
    //x->height = 1 + std::max(height(x->left),
    //    height(x->right));
    //y->height = 1 + std::max(height(y->left),
    //    height(y->right));

    // Update heights and max
    updateHeightAndMax(y);
    updateHeightAndMax(x);

    // Return new root 
    return y;
}

// Get balance factor of node N 
int getBalance(Node* N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

void updateHeightAndMax(Node* node) {
    if (!node) return;
    node->height = 1 + std::max(height(node->left), height(node->right));
    node->max = std::max(
        node->i->high,
        std::max(
            (node->left ? node->left->max : INT_MIN),
            (node->right ? node->right->max : INT_MIN)
        )
    );
}

Node* newNode(size_t pi, Interval i) {
    Node* temp = new Node;
    temp->paragraphIndex = pi;
    temp->i = new Interval(i);
    temp->max = i.high;
    temp->left = temp->right = nullptr;
    return temp;
}

// לבדוק את האיזון בכל המצבים!!!!!
Node* insertTree(Node* root, Node* n) {
	n->max = n->i->high; // Update max value of this ancestor node
    if (root == nullptr)
        return n;

    //int l = n->i->low;

    if (n->i->low < root->i->low)
        root->left = insertTree(root->left, n);
    else
        root->right = insertTree(root->right, n);

    //if (root->max < n->i->high)
    //    root->max = n->i->high;

    updateHeightAndMax(root);


    // Update height of this ancestor node 
    root->height = 1 + std::max(height(root->left),
        height(root->right));
    // Get the balance factor of this ancestor node 
    int balance = getBalance(root);

    // If this node becomes unbalanced, 
    // then there are 4 cases 

    // Left Left Case 
    if (balance > 1 && n->i->low < root->left->i->low)
        return rightRotate(root);

    // Right Right Case 
    if (balance < -1 && n->i->low > root->right->i->low)
        return leftRotate(root);

    // Left Right Case 
    if (balance > 1 && n->i->low > root->left->i->low) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case 
    if (balance < -1 && n->i->low < root->right->i->low) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

	//std::cout << getBalance(root) << std::endl;
    // Return the (unchanged) node pointer 
    return root;

}
// לסדר שאם הוא נמצא בשתיהם אז הקשת שלהם תקבל ניקוד גבוה.
bool isOverlapping(Interval i1, Interval i2) {
    return i1.low <= i2.high && i2.low <= i1.high;
}

// לבדוק מה קורה אם הוא שייך ל2 פסקאות (לא שייך לשום פסקה בודדת)
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
