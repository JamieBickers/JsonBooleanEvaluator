#pragma once
#include <string>
#include <memory>
#include "json.h"
#include "ArithmeticCondition.h"
#include "ArrayMethod.h"


class BooleanTree
{
public:
	char getBooleanOperator();
	std::string getBooleanCondition();
	std::shared_ptr<ArithmeticCondition> getArithmeticCondition();
	std::shared_ptr<ArrayMethod<bool>> getArrayMethod();
	std::shared_ptr<BooleanTree> getLeftChild();
	std::shared_ptr<BooleanTree> getRightChild();

	void setBooleanOperator(char booleanOperator);
	void setBooleanCondition(std::string condition);
	void setArithmeticCondition(std::shared_ptr<ArithmeticCondition> arithmeticCondition);
	void setArrayMethod(std::shared_ptr<ArrayMethod<bool>> arrayMethod);
	void setLeftChild(std::shared_ptr<BooleanTree> leftChild);
	void setRightChild(std::shared_ptr<BooleanTree> rightChild);

	bool evaluateNode(nlohmann::json jsonObject,
		std::map<std::string, double> additionalVariables = std::map<std::string, double>());

private:
	char booleanOperator;
	std::string booleanCondition;
	std::shared_ptr<ArithmeticCondition> arithmeticCondition;
	std::shared_ptr<ArrayMethod<bool>> arrayMethod;
	std::shared_ptr<BooleanTree> leftChild;
	std::shared_ptr<BooleanTree> rightChild;
};