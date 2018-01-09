#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/Utilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonBooleanEvaluatorTests
{		
	TEST_CLASS(UtilitiesTests)
	{
	public:
		TEST_METHOD(indexOfClosingBracketTest)
		{
			auto result = indexOfClosingBracket("zdjs(ursehfgdfgf)", 4);
			Assert::AreEqual(16, result);
		}

		TEST_METHOD(getTermsBetweenBracketsIncludingBracketsTest)
		{
			auto result = getTermsBetweenBracketsIncludingBrackets("zdjs(ur(sehfg)dfgf)fgxhdt5", 4);
			Assert::AreEqual((std::string)"(ur(sehfg)dfgf)", result);
		}

		TEST_METHOD(stringContainsCharacterTest)
		{
			auto result =  stringContainsCharacter("dfhdefg+drtdggh", '+', 5);
			Assert::AreEqual(true, result);

			result = stringContainsCharacter("dfhdefg+drtdggh", '+', 8);
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(indexOfFirstCharTest)
		{
			int indexOfFirstChar(std::string expression, char character, int position);
			auto result = indexOfFirstChar("dfhdefg+drtdggh", '+', 5);
			Assert::AreEqual(7, result);
		}

		TEST_METHOD(isNumericTest)
		{
			auto result = isNumeric("3245646g2345");
			Assert::AreEqual(false, result);

			result = isNumeric("32456462345");
			Assert::AreEqual(true, result);

			result = isNumeric("-32456h462345");
			Assert::AreEqual(false, result);

			result = isNumeric("-32456462345");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(indexOfNextOpeningBracketTest)
		{
			auto result = indexOfNextOpeningBracket("zdjs(ur(sehf)gdfgf)", 5);
			Assert::AreEqual(7, result);
		}
	};
}