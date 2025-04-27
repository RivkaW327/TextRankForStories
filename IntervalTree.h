#pragma once

struct Interval {
	int low, high;
};

struct Node {
	size_t paragraphIndex;
	Interval* i;
	int max;
	Node* left, * right;
	int height=1;
};

int height(Node* N);
Node* rightRotate(Node* y);
Node* leftRotate(Node* x);
int getBalance(Node* N);
void updateHeightAndMax(Node* node);
Node* newNode(size_t pi, Interval i);
Node* insertTree(Node* root, Node* n);
bool isOverlapping(Interval i1, Interval i2);
Node* overlapSearch(Node* root, Interval i);
void inorder(Node* root);
