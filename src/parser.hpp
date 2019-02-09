#ifndef PARSER_H
#define PARSER_H
#include "token_stream.hpp"
#include "array.hpp"
#include "token.hpp"
#include <string>
using std::string;

class Parser {
private:
  TokenStream stream_;
public:
  Parser() {};

  Parser(Array<Token>& tokens) {
    stream_ = TokenStream(tokens);
  }
  ~Parser() {

  }

  int64_t parse() {
    return addition();
  }

  int64_t addition() {
    auto token = multiplication();
    auto current = stream_.current();
    Array<string> lexemes = { "+" , "-" };
    while(stream_.match(Operator, lexemes)) {
      if (current.lexeme() == "+") {
        token += multiplication();
      } else {
        token -= multiplication();
      }
    }
    return token;
  }

  int64_t multiplication() {
    auto token = primary();
    auto current = stream_.current();
    Array<string> lexemes = { "*" , "/" };
    while(stream_.match(Operator, lexemes)) {
      if (current.lexeme() == "*") {
        token *= primary();
      } else {
        token /= primary();
      }
    }
    return token;
  }

  int64_t primary() {
    auto current = stream_.current();
    if (stream_.match(Numeral) || stream_.match(RomanNumeral)) {
      return current.literal();
    }

    string error = "Parse error: Expected primary.";
    throw error;
  }
};


#endif