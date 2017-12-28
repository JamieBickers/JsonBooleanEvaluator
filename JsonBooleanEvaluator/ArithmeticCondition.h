#pragma once

#include "ArithmeticTree.h"
#include "json.h"

class ArithmeticCondition
{
public:
	std::shared_ptr<ArithmeticTree> left;
	std::shared_ptr<ArithmeticTree> right;
	std::string comparison;

	bool evaluateCondition(nlohmann::json variables);
};

