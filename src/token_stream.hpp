#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H
#include "array.hpp"
#include "token.hpp"
#include <string>


using std::string;

class TokenStream {
private:
  array<Token> tokens_;
  int64_t position_ = 0;
public:
  TokenStream() {}

  TokenStream(array<Token>& tokens) {
    tokens_ = tokens;
  }

  ~TokenStream() {

  }

  Token& current() {
    return tokens_[position_];
  }

  Token next() {
    if (is_end()) {
      return Token(TokenType::EndOfFile, "", 0);
    }
    return tokens_[position_++];
  }

  Token prev() {
    if (position_ <= 0) {
      return Token(TokenType::EndOfFile, "", 0);
    }
    return tokens_[position_--];
  }

  Token peek(int64_t by) {
    if ((position_ + by) <= 0 || (position_ + by) >= tokens_.length()) {
      return Token(TokenType::EndOfFile, "", 0);
    }
    return tokens_[position_ + by];
  }

  bool match(TokenType type) {
    if (current().type() == type) {
      next();
      return true;
    }

    return false;
  }

  bool match(TokenType type, string lexeme) {
    if (current().type() == type && current().lexeme() == lexeme) {
      next();
      return true;
    }
    return false;
  }

  template<typename... Args>
  bool match(TokenType type, array<string>const & lexemes) {
    for (auto i = 0; i < lexemes.length(); i++) {
      if (current().type() == type && current().lexeme() == lexemes[i]) {
        next();
        return true;
      }
    }
    return false;
  }

  bool is_end() {
    return position_ >= tokens_.length();
  }
};


#endif