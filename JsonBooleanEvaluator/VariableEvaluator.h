#pragma once
#include "json.h"
#include "Utilities.h"
#include <string>
#include <vector>

std::vector<std::string> parseVariable(std::string variable);

template <typename T>
T evaluateVariable(nlohmann::json jsonObject, std::string variable)
{
	auto parsedVariable = parseVariable(variable);
	auto jsonVariable = jsonObject;
	for (size_t i = 0; i < parsedVariable.size(); i++)
	{
		if (isNumeric(parsedVariable[i])) {
			jsonVariable = jsonVariable[stod(parsedVariable[i])];
		}
		else {
			jsonVariable = jsonVariable[parsedVariable[i]];
		}
	}
	T evaluatedVariable = jsonVariable;
	return evaluatedVariable;
}

void runVariableParsingTests();
