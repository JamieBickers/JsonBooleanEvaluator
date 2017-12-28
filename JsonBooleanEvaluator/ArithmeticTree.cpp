#include "stdafx.h"
#include "json.h"
#include "ArithmeticTree.h"
#include "VariableEvaluator.h"

#include <memory>

using json = nlohmann::json;
using namespace std;

double ArithmeticTree::evaluateNode(json variables)
{
	if (!isnan(value)) {
		return value;
	}
	else if (variable != "") {
		return evaluateVariable<double>(variables, variable);
	}
	else if (arithmeticOperation == '+') {
		return leftChild->evaluateNode(variables) + rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '-') {
		return leftChild->evaluateNode(variables) - rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '*') {
		return leftChild->evaluateNode(variables) * rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '/') {
		return leftChild->evaluateNode(variables) / rightChild->evaluateNode(variables);
	}
	else {
		throw "Invalid node.";
	}
}

char ArithmeticTree::getArithmeticOperation()
{
	return arithmeticOperation;
}
double ArithmeticTree::getValue()
{
	return value;
}

string ArithmeticTree::getVariable()
{
	return variable;
}

shared_ptr<ArithmeticTree> ArithmeticTree::getLeftChild()
{
	return leftChild;
}

shared_ptr<ArithmeticTree> ArithmeticTree::getRightChild()
{
	return rightChild;
}

void ArithmeticTree::setArithmeticOperation(char arithmeticOperation)
{
	if ((!isnan(value)) || (variable != "")) {
		throw "Cannot have value or variable as well as operation.";
	}
	else if ((arithmeticOperation == '+') || (arithmeticOperation == '-')
		|| (arithmeticOperation == '*') || (arithmeticOperation == '/')) {
		this->arithmeticOperation = arithmeticOperation;
	}
	else {
		throw "Invalid operation.";
	}
}
void ArithmeticTree::setValue(double value)
{
	if (variable != "") {
		throw "Cannot have variable and value.";
	}
	else if (arithmeticOperation != NULL) {
		throw "Cannot have value and operation.";
	}
	else {
		this->value = value;
	}
}

void ArithmeticTree::setVariable(string variable)
{
	if (!isnan(value)) {
		throw "Cannot have variable and value.";
	}
	else if (arithmeticOperation != NULL) {
		throw "Cannot have varaible and operation.";
	}
	else {
		this->variable = variable;
	}
}

void ArithmeticTree::setLeftChild(shared_ptr<ArithmeticTree> leftChild)
{
	if ((variable != "") || (!isnan(value))) {
		throw "Cannot have children with either variable of value.";
	}
	else {
		this->leftChild = leftChild;
	}
}

void ArithmeticTree::setRightChild(shared_ptr<ArithmeticTree> rightChild)
{
	if ((variable != "") || (!isnan(value))) {
		throw "Cannot have children with either variable of value.";
	}
	else {
		this->rightChild = rightChild;
	}
}