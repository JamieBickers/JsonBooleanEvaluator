#include "stdafx.h"
#include "CppUnitTest.h"
//#include "ArrayMethod.h"
#include "../JsonBooleanEvaluator/parseArrayMethod.h"

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using json = nlohmann::json;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(parseArrayMethodTests)
	{
	public:
		TEST_METHOD(parseArrayMethodTest1)
		{
			auto method = "[\"a\"][\"b\"].count(x => x > 0)";
			auto result = parseArrayMethod<double>(method);
			Assert::AreEqual((std::string)"x", result->lambdaVariable);
			Assert::AreEqual((std::string)"count", result->method);
			Assert::AreEqual((std::string)"[\"a\"][\"b\"]", result->variable);
		}

		TEST_METHOD(parseArrayMethodTest2)
		{
			auto method = "[\"a\"][\"b\"].all(xyz => xyz >= 0 || xyz = -7)";
			auto result = parseArrayMethod<double>(method);
			Assert::AreEqual((std::string)"xyz", result->lambdaVariable);
			Assert::AreEqual((std::string)"all", result->method);
			Assert::AreEqual((std::string)"[\"a\"][\"b\"]", result->variable);
		}
	};
}