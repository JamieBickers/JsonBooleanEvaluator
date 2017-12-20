#include "stdafx.h"
#include "EvaluateArithmeticExpressions.h"
#include "BooleanTree.h"
#include "BooleanExpressionParser.h"
#include<iostream>

using namespace std;
int main()
{
	// print formatted booleans
	cout << boolalpha;

	runArithmeticEvaluationTests();
	runBooleanTreeTests();
	runBooleanExpressionParserTests();

	string end;
	cin >> end;

	return 0;
}