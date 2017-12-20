#include "stdafx.h"
#include "BooleanTree.h"
#include "EvaluateArithmeticExpressions.h"

#include <iostream>
#include <stack>

using namespace std;

bool evaluateTwoConditions(bool first, bool second, char booleanOperator);
bool evaluateSingleCondition(bool condition, char booleanOperator);
bool evaluateCondition(string condition);

bool BooleanTree::evaluateNode()
{
	stack <BooleanTree*> nodes;
	stack<bool> booleanValues;

	nodes.push(this);

	while (nodes.size() > 0) {
		auto currentNode = nodes.top();
		if (booleanValues.size() >= 2 && ((currentNode->booleanOperator == '&') || (currentNode->booleanOperator == '|'))) {
			bool first = booleanValues.top();
			booleanValues.pop();
			bool second = booleanValues.top();
			booleanValues.pop();
			booleanValues.push(evaluateTwoConditions(first, second, (*currentNode).booleanOperator));
			nodes.pop();
		}
		else if ((booleanValues.size() == 1)
			&& ((currentNode->booleanOperator == '!')
				|| (((currentNode->booleanOperator == NULL)) && (currentNode->condition == "")))) {
			bool evaluated = evaluateSingleCondition(booleanValues.top(), currentNode->booleanOperator);
			booleanValues.pop();
			booleanValues.push(evaluated);
			nodes.pop();
		}
		else if (currentNode->leftChild == NULL) {
			if (currentNode->rightChild == NULL) {
				booleanValues.push(evaluateCondition((*currentNode).condition));
				nodes.pop();
			}
			else {
				nodes.push(currentNode->rightChild.get());
			}
		}
		else {
			nodes.push(currentNode->leftChild.get());
			if (currentNode->rightChild != NULL) {
				nodes.push(currentNode->rightChild.get());
			}
		}
	}
	return booleanValues.top();
}

bool evaluateSingleCondition(bool condition, char booleanOperator) {
	if (booleanOperator == NULL) {
		return condition;
	}
	else if (booleanOperator == '!') {
		return !condition;
	}
	else {
		throw "Not a valid operator.";
	}
}

bool evaluateTwoConditions(bool first, bool second, char booleanOperator) {
	if (booleanOperator == '&') {
		return first && second;
	}
	else if (booleanOperator == '|') {
		return first || second;
	}
	else {
		throw "Not a valid operator.";
	}
}

bool BooleanTree::evaluateNodeUsingStack()
{
	if (leftChild == NULL && rightChild == NULL) {
		return evaluateCondition(condition);
	}
	else {
		return evaluateChildrenWithOperator();
	}
}

bool evaluateCondition(string condition)
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

	initialNode->condition = "false";
	result = initialNode->evaluateNode();
	cout << (result == false) << endl;

	initialNode->condition = "false";
	result = initialNode->evaluateNodeUsingStack();
	cout << (result == false) << endl;

	// long test
	initialNode = unique_ptr<BooleanTree>(new BooleanTree());
	initialNode->leftChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->leftChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->leftChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->leftChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->leftChild->rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild->leftChild->leftChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild->leftChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild->rightChild = make_unique<BooleanTree>(BooleanTree());
	initialNode->rightChild->rightChild->rightChild->leftChild = make_unique<BooleanTree>(BooleanTree());

	initialNode->booleanOperator = '&';
	initialNode->leftChild->booleanOperator = '|';
	initialNode->rightChild->booleanOperator = '!';
	initialNode->leftChild->leftChild->condition = "true";
	initialNode->leftChild->rightChild->booleanOperator = '|';
	initialNode->leftChild->rightChild->leftChild->condition = "true";
	initialNode->leftChild->rightChild->rightChild->condition = "false";
	initialNode->rightChild->rightChild->booleanOperator = '&';
	initialNode->rightChild->rightChild->leftChild->booleanOperator = '|';
	initialNode->rightChild->rightChild->leftChild->leftChild->condition = "true";
	initialNode->rightChild->rightChild->leftChild->rightChild->condition = "false";
	initialNode->rightChild->rightChild->rightChild->booleanOperator = NULL;
	initialNode->rightChild->rightChild->rightChild->leftChild->condition = "true";

	result = initialNode->evaluateNode();
	cout << (result == false) << endl;

	result = initialNode->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = initialNode->leftChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = initialNode->rightChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->rightChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->leftChild->leftChild->evaluateNode();
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->leftChild->rightChild->evaluateNode();
	cout << (result == false) << endl;

	result = initialNode->rightChild->rightChild->rightChild->leftChild->evaluateNode();
	cout << (result == true) << endl;
}