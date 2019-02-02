#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include "char_stream.hpp"
#include "token.hpp"
#include "roman.hpp"
#include "array.hpp"

class Scanner {
private:
  CharStream source_;
  uint64_t start_ = 0;
  Array<Token> tokens_;

  void tokenize() {
    auto ch = source_.next();
    if (std::isalpha(ch)) {
      if (is_roman_numeral(ch)) {
        tokens_.push(scan_roman_numeral());
        return;
      }
    }

    if (is_numeral(ch)) {
      tokens_.push(scan_numeral());
      return;
    }

    if(is_operator(ch)) {
      tokens_.push(Token(Operator, string(1, ch), 0));
      return;
    }

    if(std::iswspace(ch) || std::isspace(ch)) {
      return;
    }

    tokens_.push(Token(Error, "Invalid character '" + string(1, ch) + "'", 0));
  }

  const Token scan_roman_numeral() {
    while (
      is_roman_numeral(source_.current()) && 
      !source_.is_eos()
    ) {
      source_.next();
    }

    auto lexeme = source_.range(start_);
    int literal = 0;
    
    try {
      literal = roman(lexeme);
    } catch(string error) {
      return Token(Error, error, 0);
    }
    return Token(
      RomanNumeral, 
      lexeme,
      literal
    );
  }

  const Token scan_numeral() {
    while(
      is_numeral(source_.current()) && 
      !source_.is_eos()
    ) {
      source_.next();
    }

    auto lexeme = source_.range(start_);
    auto literal = std::stoll(lexeme);
    return Token(
      Numeral,
      lexeme,
      literal
    );
  }

  bool is_roman_numeral(const char& ch) {
    switch (ch) {
        case 'i':
        case 'v':
        case 'x':
        case 'l':
        case 'c':
        case 'd':
        case 'm':
          return true;
        default:
          break;
      }
      return false;
  }

  bool is_numeral(const char& ch) {
    return ch >= '0' && ch <= '9';
  }
  
  bool is_operator(const char& ch) {
    switch (ch) {
      case '+':
      case '-':
      case '*':
      case '/':
        return true;
      default:
        return false;
    }
  }

public:
  Scanner() {
    tokens_ = Array<Token>();
  }
  Scanner(const std::string& source) {
    source_ = CharStream(source);
    tokens_ = Array<Token>();
  }
  Scanner(const Scanner& scanner) {
    start_ = scanner.start_;
    source_ = scanner.source_;
    tokens_ = scanner.tokens_;
  }

  ~Scanner() {
  }

  auto scan() {
    while (!source_.is_eos()) {
      // Mark the beginning of the lexeme
      start_ = source_.position();
      tokenize();
    }
    tokens_.push(Token(EndOfFile, "\0", 0));
    return tokens_;
  }
};


#endif