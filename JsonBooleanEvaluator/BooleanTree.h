#pragma once
#include<string>
#include<memory>

class BooleanTree
{
public:
	char booleanOperator;
	std::string condition;
	std::unique_ptr<BooleanTree> leftChild;
	std::unique_ptr<BooleanTree> rightChild;

	bool evaluateNode();
	bool evaluateNodeUsingStack();

private:
	bool evaluateChildrenWithOperator();
	bool evaluateNotOperator();
	bool evaluateEmptyOperator();
};

void runBooleanTreeTests();