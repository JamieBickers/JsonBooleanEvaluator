#pragma once
#include "BooleanTree.h"
#include <string>
#include <memory>

std::shared_ptr<BooleanTree> parseBooleansExpressionToTree(std::string expression);
void runBooleanExpressionParserTests();