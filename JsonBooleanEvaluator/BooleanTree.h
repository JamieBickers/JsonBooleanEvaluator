#pragma once
#include<string>

class BooleanTree
{
public:
	char booleanOperator;
	std::string condition;
	BooleanTree* leftChild;
	BooleanTree* rightChild;

	bool evaluateNode();

private:
	bool evaluateCondition();
	bool evaluateChildrenWithOperator();
	bool evaluateNotOperator();
	bool evaluateEmptyOperator();
};

void runBooleanTreeTests();