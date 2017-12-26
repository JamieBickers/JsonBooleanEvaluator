#include "stdafx.h"
#include "BooleanTree.h"
#include "EvaluateArithmeticExpressions.h"
#include "json.h"
#include "VariableEvaluator.h"

#include <iostream>
#include <stack>
#include <string>
#include <regex>

using namespace std;
using json = nlohmann::json;

bool evaluateTwoConditions(bool first, bool second, char booleanOperator);
bool evaluateSingleCondition(bool condition, char booleanOperator);
bool evaluateCondition(string condition, json jsonObject);

bool evaluateNodeRecursive(shared_ptr<BooleanTree> node, json jsonObject)
{
	if ((node->leftChild) == NULL && (node->rightChild == NULL)) {
		return evaluateCondition(node->condition, jsonObject);
	}
	else if (node->leftChild == NULL) {
		return evaluateSingleCondition(evaluateNodeRecursive(node->rightChild, jsonObject), node->booleanOperator);
	}
	else if (node->rightChild == NULL) {
		return evaluateSingleCondition(evaluateNodeRecursive(node->leftChild , jsonObject), node->booleanOperator);
	}
	else {
		bool evaluatedLeftChild = evaluateNodeRecursive(node->leftChild, jsonObject);
		bool evaluatedRightChild = evaluateNodeRecursive(node->rightChild, jsonObject);
		return evaluateTwoConditions(evaluatedLeftChild, evaluatedRightChild, node->booleanOperator);
	}
}

bool BooleanTree::evaluateNode(json jsonObject)
{
	stack <BooleanTree*> nodes;
	stack<bool> booleanValues;

	nodes.push(this);

	while (nodes.size() > 0) {
		auto currentNode = nodes.top();
		if (booleanValues.size() >= 2
			&& ((currentNode->booleanOperator == '&') || (currentNode->booleanOperator == '|'))) {
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
				booleanValues.push(evaluateCondition(currentNode->condition, jsonObject));
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

bool evaluateCondition(string condition, json jsonObject)
{
	regex arithmeticOperatorRegex("<|>|=");
	if (condition == "true") {
		return true;
	}
	else if (condition == "false") {
		return false;
	}
	else if (regex_search(condition, arithmeticOperatorRegex)) {
		return evaluateArithmeticCondition(condition, jsonObject);
	}
	else {
		return evaluateVariable<bool>(jsonObject, condition);
	}
}

void runBooleanTreeTests()
{
	cout << "Running BooleanTree tests." << endl;

	auto varables = map<string, double>();

	auto initialNode = unique_ptr<BooleanTree>(new BooleanTree());
	initialNode->condition = "true";
	bool result = initialNode->evaluateNode(varables);
	cout << (result == true) << endl;

	initialNode->condition = "false";
	result = initialNode->evaluateNode(varables);
	cout << (result == false) << endl;

	initialNode->condition = "false";
	result = initialNode->evaluateNode(varables);
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

	result = initialNode->evaluateNode(varables);
	cout << (result == false) << endl;

	result = initialNode->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->rightChild->evaluateNode(varables);
	cout << (result == false) << endl;

	result = initialNode->leftChild->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->leftChild->rightChild->rightChild->evaluateNode(varables);
	cout << (result == false) << endl;

	result = initialNode->rightChild->rightChild->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->rightChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->leftChild->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;

	result = initialNode->rightChild->rightChild->leftChild->rightChild->evaluateNode(varables);
	cout << (result == false) << endl;

	result = initialNode->rightChild->rightChild->rightChild->leftChild->evaluateNode(varables);
	cout << (result == true) << endl;
}