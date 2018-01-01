#pragma once

#include <string>

int indexOfClosingBracket(std::string expression, int position);
std::string getTermsBetweenBracketsIncludingBrackets(std::string expression, int position);
bool stringContainsCharacter(std::string expression, char character, int startingPosition = 0);
int indexOfFirstChar(std::string expression, char character, int position);
bool isNumeric(std::string possibleNumber);
int indexOfNextOpeningBracket(std::string expression, int position);