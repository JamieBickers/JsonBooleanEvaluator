#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/BooleanExpressionParser.h"
#include "../JsonBooleanEvaluator/json.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using json = nlohmann::json;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(BooleanTreeTests)
	{
	public:
		TEST_METHOD(parseBooleansExpressionToTreeTest1)
		{
			json variables;
			std::string booleanexpression = "true";
			auto tree = parseBooleansExpressionToTree(booleanexpression);
			bool result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}
	};
}