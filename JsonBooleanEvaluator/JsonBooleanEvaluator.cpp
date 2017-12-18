#include "stdafx.h"
#include "EvaluateArithmeticExpressions.h"
#include "BooleanTree.h"
#include<iostream>

using namespace std;
int main()
{
	// print formatted booleans
	cout << boolalpha;

	runArithmeticEvaluationTests();
	runBooleanTreeTests();

	return 0;
}