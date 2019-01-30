#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {
  None = 0,
  RomanNumeral = 1,
  Numeral = 2,
  Operator = 3,
  EndOfFile = -1,
  Error = -2
};

class Token {
private:
  TokenType type_ = TokenType::None;
  std::string lexeme_ = "";
  uint64_t literal_ = 0;
public:
  Token(TokenType type, std::string lexeme, uint64_t literal) {
    type_ = type;
    lexeme_ = lexeme;
    literal_ = literal;
  }

  ~Token() {
    type_ = TokenType::None;
    lexeme_ = "";
    literal_ = 0;
  }

  TokenType type() {
    return type_;
  }

  std::string lexeme() {
    return lexeme_;
  }

  uint64_t literal() {
    return literal_;
  }
};


#endif