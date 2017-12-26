#pragma once
#include <string>
#include <memory>
#include "json.h"

class BooleanTree
{
public:
	char booleanOperator;
	std::string condition;
	std::shared_ptr<BooleanTree> leftChild;
	std::shared_ptr<BooleanTree> rightChild;

	bool evaluateNode(nlohmann::json jsonObject);
};

bool evaluateNodeRecursive(std::shared_ptr<BooleanTree> tree, nlohmann::json jsonObject);
void runBooleanTreeTests();