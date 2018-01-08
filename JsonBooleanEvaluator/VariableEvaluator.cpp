#include "stdafx.h"
#include "json.h"
#include "Utilities.h"
#include "VariableEvaluator.h"

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using json = nlohmann::json;

std::vector<std::string> parseVariable(std::string variable)
{
	std::regex betweenSquareBracketsRegex("\\[([^\\]]+)\\]");
	auto matchesBegin = std::sregex_iterator(variable.begin(), variable.end(), betweenSquareBracketsRegex);
	auto matchesEnd = std::sregex_iterator();
	std::vector<std::string> parsedVariable;
	for (std::sregex_iterator i = matchesBegin; i != matchesEnd; i++) {
		std::smatch match = *i;
		std::string matchString = match.str();
		std::regex quoteRegex("(.*)\"(.*)");
		if (regex_match(matchString, quoteRegex)) {
			parsedVariable.push_back(matchString.substr(2, matchString.size() - 4));
		}
		else {
			parsedVariable.push_back(matchString.substr(1, matchString.size() - 2));
		}
	}
	return parsedVariable;
}

void runVariableParsingTests()
{
	std::cout << "Running variables parsing tests." << std::endl;

	auto myJson = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
	auto parsedJson = json::parse(myJson);
	auto variable = "[\"c\"][\"d\"]";
	auto result = evaluateVariable<int>(parsedJson, variable);
	std::cout << (result == 10) << std::endl;

	variable = "[\"c\"][\"e\"][1]";
	result = evaluateVariable<int>(parsedJson, variable);
	std::cout << (result == 2) << std::endl;

	variable = "[\"c\"][\"f\"]";
	bool result1 = evaluateVariable<bool>(parsedJson, variable);
	std::cout << (result1 == true) << std::endl;
}