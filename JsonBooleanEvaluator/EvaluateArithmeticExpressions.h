#pragma once
#include <string>
#include <string>
#include "json.h"
#include "ArithmeticTree.h"

std::shared_ptr<ArithmeticTree> parseArithmeticExpressionToTree(std::string expression);
void runArithmeticEvaluationTests();