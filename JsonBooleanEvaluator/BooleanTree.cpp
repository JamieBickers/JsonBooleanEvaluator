#include "stdafx.h"
#include "BooleanTree.h"
#include "EvaluateArithmeticExpressions.h"

// classes example
#include <iostream>
using namespace std;

bool BooleanTree::evaluateNode()
{
	if (leftChild == NULL && rightChild == NULL) {
		return evaluateCondition();
	}
	else {
		return evaluateChildrenWithOperator();
	}
}

bool BooleanTree::evaluateCondition()
{
	if (condition == "true") {
		return true;
	}
	else if (condition == "false") {
		return false;
	}
}

bool BooleanTree::evaluateChildrenWithOperator()
{
	if (leftChild == NULL || rightChild == NULL) {
		if (booleanOperator == '!') {
			return evaluateNotOperator();
		}
		else if (booleanOperator == NULL) {
			return evaluateEmptyOperator();
		}
	}

	bool left = (*leftChild).evaluateNode();
	bool right = (*rightChild).evaluateNode();

	if (booleanOperator == '&') {
		return left && right;
	}
	else if (booleanOperator == '|') {
		return left || right;
	}
	else {
		throw "Invalid operation.";
	}
}

bool BooleanTree::evaluateNotOperator()
{
	if (booleanOperator != '!') {
		throw "Invalid operation.";
	}
	else if (leftChild == NULL) {
		return !(*rightChild).evaluateNode();
	}
	else if (rightChild == NULL) {
		return !(*leftChild).evaluateNode();
	}
	else {
		throw "cannot use not if both children are not null.";
	}
}

bool BooleanTree::evaluateEmptyOperator() {
	if (booleanOperator != NULL) {
		throw "Invalid operation.";
	}
	else if (leftChild == NULL) {
		return (*rightChild).evaluateNode();
	}
	else if (rightChild == NULL) {
		return (*leftChild).evaluateNode();
	}
	else {
		throw "cannot use empty operator if both children are not null.";
	}
}

void runBooleanTreeTests()
{
	cout << "Running BooleanTree tests." << endl;

	BooleanTree initialNode = BooleanTree();
	initialNode.condition = "true";
	bool result = initialNode.evaluateNode();
	cout << (result == true) << endl;

	initialNode = BooleanTree();
	initialNode.condition = "false";
	result = initialNode.evaluateNode();
	cout << (result == false) << endl;

	// long test
	initialNode = BooleanTree();
	BooleanTree node1 = BooleanTree();
	BooleanTree node2 = BooleanTree();
	BooleanTree node3 = BooleanTree();
	BooleanTree node4 = BooleanTree();
	BooleanTree node5 = BooleanTree();
	BooleanTree node6 = BooleanTree();
	BooleanTree node7 = BooleanTree();
	BooleanTree node8 = BooleanTree();
	BooleanTree node9 = BooleanTree();
	BooleanTree node10 = BooleanTree();
	BooleanTree node11 = BooleanTree();
	BooleanTree node12 = BooleanTree();

	node3.condition = "true";
	node6.condition = "true";
	node7.condition = "false";
	node10.condition = "true";
	node11.condition = "false";
	node12.condition = "true";

	initialNode.booleanOperator = '&';
	node1.booleanOperator = '|';
	node4.booleanOperator = '|';
	node5.booleanOperator = '&';
	node8.booleanOperator = '|';

	node2.booleanOperator = '!';
	node9.booleanOperator = NULL;

	initialNode.leftChild = &node1;
	initialNode.rightChild = &node2;

	node1.leftChild = &node3;
	node1.rightChild = &node4;
	node4.leftChild = &node6;
	node4.rightChild = &node7;
	node2.rightChild = &node5;
	node5.leftChild = &node8;
	node5.rightChild = &node9;
	node8.leftChild = &node10;
	node8.rightChild = &node11;
	node9.leftChild = &node12;

	result = initialNode.evaluateNode();
	cout << (result == false) << endl;

	result = node1.evaluateNode();
	cout << (result == true) << endl;

	result = node2.evaluateNode();
	cout << (result == false) << endl;

	result = node3.evaluateNode();
	cout << (result == true) << endl;

	result = node4.evaluateNode();
	cout << (result == true) << endl;

	result = node5.evaluateNode();
	cout << (result == true) << endl;

	result = node6.evaluateNode();
	cout << (result == true) << endl;

	result = node7.evaluateNode();
	cout << (result == false) << endl;

	result = node8.evaluateNode();
	cout << (result == true) << endl;

	result = node9.evaluateNode();
	cout << (result == true) << endl;

	result = node10.evaluateNode();
	cout << (result == true) << endl;

	result = node11.evaluateNode();
	cout << (result == false) << endl;

	result = node12.evaluateNode();
	cout << (result == true) << endl;

	string end;
	cin >> end;
}