#pragma once

#include "json.h"

#include <string>
#include <memory>

class ArithmeticTree
{
public:
	// getters
	char getArithmeticOperation();
	double getValue();
	std::string getVariable();
	std::shared_ptr<ArithmeticTree> getLeftChild();
	std::shared_ptr<ArithmeticTree> getRightChild();

	// setters
	void setArithmeticOperation(char arithmeticOperation);
	void setValue(double value);
	void setVariable(std::string variable);
	void setLeftChild(std::shared_ptr<ArithmeticTree> leftChild);
	void setRightChild(std::shared_ptr<ArithmeticTree> rightChild);

	double evaluateNode(nlohmann::json variables);

private:
	char arithmeticOperation;
	double value = NAN;
	std::string variable;
	std::shared_ptr<ArithmeticTree> leftChild;
	std::shared_ptr<ArithmeticTree> rightChild;
};