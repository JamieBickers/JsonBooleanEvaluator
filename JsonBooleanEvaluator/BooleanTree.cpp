#include "stdafx.h"
#include "BooleanTree.h"
#include "EvaluateArithmeticExpressions.h"
#include "json.h"
#include "VariableEvaluator.h"

#include <iostream>
#include <stack>
#include <string>
#include <regex>
#include <queue>

using namespace std;
using json = nlohmann::json;

bool evaluateTwoConditions(bool first, bool second, char booleanOperator);
bool evaluateSingleCondition(bool condition, char booleanOperator);
bool evaluateCondition(string condition, json jsonObject);

char BooleanTree::getBooleanOperator()
{
	return this->booleanOperator;
}

string BooleanTree::getBooleanCondition()
{
	return this->booleanCondition;
}

shared_ptr<ArithmeticCondition> BooleanTree::getArithmeticCondition()
{
	return this->arithmeticCondition;
}

shared_ptr<BooleanTree> BooleanTree::getLeftChild()
{
	return this->leftChild;
}

shared_ptr<BooleanTree> BooleanTree::getRightChild()
{
	return this->rightChild;
}

void BooleanTree::setBooleanOperator(char booleanOperator)
{
	if ((booleanCondition != "") || (arithmeticCondition != NULL)) {
		throw "Cannot have condition and operator.";
	}
	else {
		this->booleanOperator = booleanOperator;
	}
}

void BooleanTree::setBooleanCondition(string condition)
{
	if (booleanOperator != NULL) {
		throw "Cannot have condition and operator.";
	}
	else if (arithmeticCondition != NULL) {
		throw "Cannot have boolean condition and arithmetic condition.";
	}
	else {
		this->booleanCondition = condition;
	}
}

void BooleanTree::setArithmeticCondition(shared_ptr<ArithmeticCondition> condition)
{
	if (booleanOperator != NULL) {
		throw "Cannot have condition and operator.";
	}
	else if (booleanCondition != "") {
		throw "Cannot have boolean condition and arithmetic condition.";
	}
	else {
		this->arithmeticCondition = condition;
	}
}

void BooleanTree::setLeftChild(shared_ptr<BooleanTree> leftChild)
{
	if ((booleanCondition != "") || (arithmeticCondition != NULL)) {
		throw "Cannot have condition and children.";
	}
	else {
		this->leftChild = leftChild;
	}
}

void BooleanTree::setRightChild(shared_ptr<BooleanTree> rightChild)
{
	if ((booleanCondition != "") || (arithmeticCondition != NULL)) {
		throw "Cannot have condition and children.";
	}
	else {
		this->rightChild = rightChild;
	}
}

bool BooleanTree::evaluateNode(json variables)
{
	if ((leftChild) == NULL && (rightChild == NULL)) {
		if (arithmeticCondition != NULL) {
			return arithmeticCondition->evaluateCondition(variables);
		}
		else {
			return evaluateCondition(booleanCondition, variables);
		}
	}
	else if (leftChild == NULL) {
		return evaluateSingleCondition(rightChild->evaluateNode(variables), booleanOperator);
	}
	else if (rightChild == NULL) {
		return evaluateSingleCondition(leftChild->evaluateNode(variables), booleanOperator);
	}
	else {
		bool evaluatedLeftChild = leftChild->evaluateNode(variables);
		bool evaluatedRightChild = rightChild->evaluateNode(variables);
		return evaluateTwoConditions(evaluatedLeftChild, evaluatedRightChild, booleanOperator);
	}
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
	if (condition == "true") {
		return true;
	}
	else if (condition == "false") {
		return false;
	}
	else {
		return evaluateVariable<bool>(jsonObject, condition);
	}
}

void runBooleanTreeTests()
{
	cout << "Running BooleanTree tests." << endl;

	json variables;

	auto initialNode = shared_ptr<BooleanTree>(new BooleanTree());
	initialNode->setBooleanCondition("true");
	bool result = initialNode->evaluateNode(variables);
	cout << (result == true) << endl;

	initialNode->setBooleanCondition("false");
	result = initialNode->evaluateNode(variables);
	cout << (result == false) << endl;

	initialNode->setBooleanCondition("false");
	result = initialNode->evaluateNode(variables);
	cout << (result == false) << endl;
}