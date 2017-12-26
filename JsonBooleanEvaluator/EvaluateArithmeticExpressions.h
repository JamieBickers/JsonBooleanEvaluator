#pragma once
#include <string>
#include <string>
#include "json.h"

double evaluateArithmeticExpression(std::string expression, nlohmann::json jsonObject);
bool evaluateArithmeticCondition(std::string condition, nlohmann::json jsonObject);
void runArithmeticEvaluationTests();