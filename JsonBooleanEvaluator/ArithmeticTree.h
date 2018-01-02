#pragma once

#include "json.h"
#include "ArrayMethod.h"

#include <string>
#include <memory>

class ArithmeticTree
{
public:
	// getters
	char getArithmeticOperation();
	double getValue();
	std::string getVariable();
	std::shared_ptr<ArrayMethod<double>> getArrayMethod();
	std::shared_ptr<ArithmeticTree> getLeftChild();
	std::shared_ptr<ArithmeticTree> getRightChild();

	// setters
	void setArithmeticOperation(char arithmeticOperation);
	void setValue(double value);
	void setVariable(std::string variable);
	void setArrayMethod(std::shared_ptr<ArrayMethod<double>> arrayMethod);
	void setLeftChild(std::shared_ptr<ArithmeticTree> leftChild);
	void setRightChild(std::shared_ptr<ArithmeticTree> rightChild);

	double evaluateNode(nlohmann::json variables,
		std::map<std::string, double> additionalVariables = std::map<std::string, double>());

private:
	char arithmeticOperation;
	double value = NAN;
	std::string variable;
	std::shared_ptr<ArrayMethod<double>> arrayMethod;
	std::shared_ptr<ArithmeticTree> leftChild;
	std::shared_ptr<ArithmeticTree> rightChild;
};