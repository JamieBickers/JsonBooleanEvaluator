#pragma once
#include "json.h"
#include <string>

template <class T>
T evaluateVariable(nlohmann::json jsonObject, std::string variable);
void runVariableParsingTests();
