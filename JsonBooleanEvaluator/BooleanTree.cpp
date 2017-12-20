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
	else {
		return evaluateArithmeticExpression(condition);
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

	bool left = leftChild->evaluateNode();
	bool right = rightChild->evaluateNode();

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
		return !rightChild->evaluateNode();
	}
	else if (rightChild == NULL) {
		return !leftChild->evaluateNode();
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
		return rightChild->evaluateNode();
	}
	else if (rightChild == NULL) {
		return leftChild->evaluateNode();
	}
	else {
		throw "cannot use empty operator if both children are not null.";
	}
}

void runBooleanTreeTests()
{
	cout << "Running BooleanTree tests." << endl;

	auto initialNode = unique_ptr<BooleanTree>(new BooleanTree());
	initialNode->condition = "true";
	bool result = initialNode->evaluateNode();
	cout << (result == true) << endl;

	auto newInitialNode = BooleanTree();
	newInitialNode.condition = "false";
	result = newInitialNode.evaluateNode();
	cout << (result == false) << endl;

	// long test
	newInitialNode = BooleanTree();
	newInitialNode.leftChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.leftChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.leftChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.leftChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.leftChild->rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild->leftChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild->leftChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	newInitialNode.rightChild->rightChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());

	newInitialNode.booleanOperator = '&';
	newInitialNode.leftChild->booleanOperator = '|';
	newInitialNode.rightChild->booleanOperator = '!';
	newInitialNode.leftChild->leftChild->condition = "true";
	newInitialNode.leftChild->rightChild->booleanOperator = '|';
	newInitialNode.leftChild->rightChild->leftChild->condition = "true";
	newInitialNode.leftChild->rightChild->rightChild->condition = "false";
	newInitialNode.rightChild->rightChild->booleanOperator = '&';
	newInitialNode.rightChild->rightChild->leftChild->booleanOperator = '|';
	newInitialNode.rightChild->rightChild->leftChild->leftChild->condition = "true";
	newInitialNode.rightChild->rightChild->leftChild->rightChild->condition = "false";
	newInitialNode.rightChild->rightChild->rightChild->booleanOperator = NULL;
	newInitialNode.rightChild->rightChild->rightChild->leftChild->condition = "true";

	result = newInitialNode.evaluateNode();
	cout << (result == false) << endl;

	result = newInitialNode.leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = newInitialNode.leftChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.leftChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.rightChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.leftChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.leftChild->rightChild->rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = newInitialNode.rightChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.rightChild->rightChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.rightChild->rightChild->leftChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = newInitialNode.rightChild->rightChild->leftChild->rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = newInitialNode.rightChild->rightChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	initialNode.reset();
}