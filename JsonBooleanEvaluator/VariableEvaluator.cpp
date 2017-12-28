#include "stdafx.h"
#include "json.h"
#include "Utilities.h"
#include "VariableEvaluator.h"

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using json = nlohmann::json;
using namespace std;

vector<string> parseVariable(string variable)
{
	regex betweenSquareBracketsRegex("\\[([^\\]]+)\\]");
	auto matchesBegin = sregex_iterator(variable.begin(), variable.end(), betweenSquareBracketsRegex);
	auto matchesEnd = sregex_iterator();
	vector<string> parsedVariable;
	for (sregex_iterator i = matchesBegin; i != matchesEnd; i++) {
		smatch match = *i;
		string matchString = match.str();
		regex quoteRegex("(.*)\"(.*)");
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
	cout << "Running variables parsing tests." << endl;

	auto myJson = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
	auto parsedJson = json::parse(myJson);
	auto variable = "[\"c\"][\"d\"]";
	auto result = evaluateVariable<int>(parsedJson, variable);
	cout << (result == 10) << endl;

	variable = "[\"c\"][\"e\"][1]";
	result = evaluateVariable<int>(parsedJson, variable);
	cout << (result == 2) << endl;

	variable = "[\"c\"][\"f\"]";
	bool result1 = evaluateVariable<bool>(parsedJson, variable);
	cout << (result1 == true) << endl;
}