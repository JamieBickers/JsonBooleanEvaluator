#include "stdafx.h"
#include "json.h"
#include "ArithmeticTree.h"
#include "VariableEvaluator.h"
#include "ArrayMethod.h"

#include <memory>

using json = nlohmann::json;
using namespace std;

double ArithmeticTree::evaluateNode(json variables, map<string, double> additionalVariables)
{
	if (!isnan(value)) {
		return value;
	}
	else if (variable != "") {
		return evaluateVariable<double>(variables, variable, additionalVariables);
	}
	else if (arrayMethod != NULL) {
		return arrayMethod->evaluateMethod(variables);
	}
	else if (arithmeticOperation == '+') {
		return leftChild->evaluateNode(variables, additionalVariables) + rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '-') {
		return leftChild->evaluateNode(variables, additionalVariables) - rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '*') {
		return leftChild->evaluateNode(variables, additionalVariables) * rightChild->evaluateNode(variables);
	}
	else if (arithmeticOperation == '/') {
		return leftChild->evaluateNode(variables, additionalVariables) / rightChild->evaluateNode(variables);
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

shared_ptr<ArrayMethod<double>> ArithmeticTree::getArrayMethod()
{
	return arrayMethod;
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
	if ((!isnan(value)) || (variable != "") || (arrayMethod != NULL)) {
		throw "Cannot have value or variable or array method as well as operation.";
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
	else if (arrayMethod != NULL) {
		throw "Cannot have value and array method.";
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
	else if (arrayMethod != NULL) {
		throw "Cannot have variable and array method.";
	}
	else {
		this->variable = variable;
	}
}

void ArithmeticTree::setArrayMethod(shared_ptr<ArrayMethod<double>> arrayMethod)
{
	if (!isnan(value)) {
		throw "Cannot have array method and value.";
	}
	else if (arithmeticOperation != NULL) {
		throw "Cannot have array method and operation.";
	}
	else if (variable != "") {
		throw "Cannot have variable and array method.";
	}
	else {
		this->arrayMethod = arrayMethod;
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