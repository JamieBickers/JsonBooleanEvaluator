using JsonBooleanEvaluator;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace JsonBooleanEvaluatorTests
{
    [TestClass]
    public class TokenizerTests
    {
        [TestMethod]
        public void Tokenzier_Test()
        {
            string condition = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"c\"][\"e\"].count(x => true) == 5) &&  [\"c\"][\"f\"]";
            var tokenizer = new Tokenizer(condition);
            var result = tokenizer.Tokenize();
        }

        private void WriteTokensToXmlFile(string filePath, List<Token> tokens)
        {
            var builder = new StringBuilder();
            foreach (var token in tokens)
            {
                builder.Append($"<{token.Type}>{token.Value}</{token.Type}>\n");
            }

            File.WriteAllText(@"C:\Users\bicke\OneDrive\Desktop\JsonBooleanEvaluator\TestResults.txt", builder.ToString());
        }
    }
}
