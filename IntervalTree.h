#pragma once

#include <memory>

struct Interval {

	int low, high;

};

class Node

{

public:
	int Getheight();
	static std::unique_ptr<Node> rightRotate(std::unique_ptr<Node> y);
	static std::unique_ptr<Node> leftRotate(std::unique_ptr<Node> x);	int getBalance();
	void updateHeightAndMax();
	static std::unique_ptr<Node> insertTree(std::unique_ptr<Node> root, std::unique_ptr<Node> n);
	static bool isOverlapping(Interval i1, Interval i2);
	Node* overlapSearch(Interval i);
	void inorder();
	int GetParagraphIndex();
	Node(size_t paragraphIndex, Interval i);


private:
	size_t paragraphIndex;
	Interval i;
	int max;
	std::unique_ptr<Node> left, right;
	int height;
};
