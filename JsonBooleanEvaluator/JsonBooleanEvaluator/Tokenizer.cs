using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace JsonBooleanEvaluator
{
    public class Tokenizer
    {
        private readonly string booleanCondition;
        private int currentPosition = 0;

        public Tokenizer(string booleanCondition)
        {
            this.booleanCondition = booleanCondition;
        }

        public List<Token> Tokenize()
        {
            var tokens = new List<Token>();

            while (currentPosition < booleanCondition.Length)
            {
                tokens.Add(GetNextToken());
            }

            return tokens;
        }

        private Token GetNextToken()
        {
            if (booleanCondition[currentPosition] == ' ')
            {
                currentPosition++;
                return GetNextToken();
            }
            else if (NextTokenIsSymbol())
            {
                return GetNextSymbolToken();
            }
            else if (NextTokenIsKeyword())
            {
                return GetNextKeywordToken();
            }
            else if (NextTokenIsIdentifier())
            {
                return GetNextIdentifierToken();
            }
            else if (NextTokenIsConstant())
            {
                return GetNextConstantToken();
            }
            else
            {
                throw new ArgumentException();
            }
        }

        private bool NextTokenIsConstant()
        {
            return Regex.IsMatch(booleanCondition.Substring(currentPosition), "^((-?[0-9]+)|(\"[^\"]*\"))");
        }

        private bool NextTokenIsKeyword()
        {
            return Regex.IsMatch(booleanCondition.Substring(currentPosition), "^(true|false|any|all|count)");
        }

        private bool NextTokenIsIdentifier()
        {
            char[] initialIdentifierCharacters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ".ToCharArray();
            return initialIdentifierCharacters.Contains(booleanCondition[currentPosition]);
        }

        private bool NextTokenIsSymbol()
        {
            char[] symbolCharacters = "()[]+-*/<>=&|!,.".ToCharArray();
            char currentCharacter = booleanCondition[currentPosition];

            return symbolCharacters.Contains(currentCharacter);
        }

        private Token GetNextConstantToken()
        {
            if (booleanCondition[currentPosition] == '"')
            {
                var stringConstantMatch = Regex.Match(booleanCondition.Substring(currentPosition), "^\"[^\"]*\"");
                string value = stringConstantMatch.Value;
                var token = new Token()
                {
                    Type = "string constant",
                    Value = value
                };
                currentPosition += value.Length;
                return token;
            }
            else
            {
                var integerConstantMatch = Regex.Match(booleanCondition.Substring(currentPosition), "[0-9]*");
                string value = integerConstantMatch.Value;
                var token = new Token()
                {
                    Type = "integer constant",
                    Value = value
                };
                currentPosition += value.Length;
                return token;
            }
        }

        private Token GetNextKeywordToken()
        {
            var keywordMatch = Regex.Match(booleanCondition.Substring(currentPosition), "^(true|false|any|all|count)");
            string value = keywordMatch.Value;
            currentPosition += value.Length;
            return new Token()
            {
                Type = "keyword",
                Value = value
            };
        }

        private Token GetNextIdentifierToken()
        {
            var identifierMatch = Regex.Match(booleanCondition.Substring(currentPosition), "^[_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]");
            string value = identifierMatch.Value;
            currentPosition += value.Length;
            return new Token()
            {
                Type = "identifier",
                Value = value
            };
        }

        private Token GetNextSymbolToken()
        {
            char[] singleCharacterSymbols = "()[]+-*/!,.".ToCharArray();
            char[] doubleCharacterSymbols = "=|&".ToCharArray();
            char[] singleOrDoubleCharacterSymbols = "<>".ToCharArray();

            // => is more complex so handle separately
            if (booleanCondition.Substring(currentPosition) == "=>")
            {
                currentPosition++;
                return new Token()
                {
                    Type = "symbol",
                    Value = "=>"
                };
            }

            if (singleCharacterSymbols.Contains(booleanCondition[currentPosition]))
            {
                currentPosition++;
                return new Token()
                {
                    Type = "symbol",
                    Value = booleanCondition[currentPosition - 1].ToString()
                };
            }
            else if (doubleCharacterSymbols.Contains(booleanCondition[currentPosition]))
            {
                currentPosition += 2;
                return new Token()
                {
                    Type = "symbol",
                    Value = booleanCondition.Substring(currentPosition - 2, 2)
                };
            }
            else if (singleOrDoubleCharacterSymbols.Contains(booleanCondition[currentPosition]))
            {
                if (booleanCondition[currentPosition + 1] == '=')
                {
                    currentPosition += 2;
                    return new Token()
                    {
                        Type = "symbol",
                        Value = booleanCondition.Substring(currentPosition - 2, 2)
                    };
                }
                else
                {
                    currentPosition++;
                    return new Token()
                    {
                        Type = "symbol",
                        Value = booleanCondition[currentPosition - 1].ToString()
                    };
                }
            }
            else
            {
                throw new ArgumentException();
            }
        }
    }
}
