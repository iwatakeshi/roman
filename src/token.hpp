#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <iostream>
using std::ostream;

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
  Token() {}
  Token(TokenType type, const std::string& lexeme, uint64_t literal) {
    type_ = type;
    lexeme_ = lexeme;
    literal_ = literal;
  }

  ~Token() {
    type_ = TokenType::None;
    lexeme_ = "";
    literal_ = 0;
  }

  friend ostream& operator << (ostream& os, Token const& token) {
    os << "(" << token.lexeme_ << ", " << token.literal_ << ")";
    return os;
  }

  std::string to_string() {
    return "(" + lexeme_ + ", " + std::to_string(literal_) + ")";
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