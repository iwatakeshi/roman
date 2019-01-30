#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include "char_stream.hpp"
#include "token.hpp"
#include "translate.hpp"
#include "array.hpp"

class Scanner {
private:
  CharStream source_;
  uint64_t start_ = 0;

  const Token& token() {

    if (source_.is_eos()) {
      return Token(EndOfFile, "\0", 0);
    }

    start_ = source_.position();
    const auto ch = source_.next();
    if (std::tolower(ch) >= 'a' && std::tolower(ch) <='z') {
      if (is_roman_numeral(ch)) {
        return scan_roman_numeral();
      }
    }

    if (is_numeral(ch)) {
      return scan_numeral();
    }

    switch (ch) {
      case '+':
      case '-':
      case '*':
      case '/':
        return Token(Operator, get_lexeme(), 0);
      default:
        break;
    }

    return Token(Error, "Invalid character '" + ch + '\'', 0);
  }

  const Token& scan_roman_numeral() {
    auto ch = source_.next();
    while(!std::iswspace(ch) && is_roman_numeral(ch) && !source_.is_eos()) {
      ch = source_.next();
    }
    auto lexeme = get_lexeme();
    return Token(
      RomanNumeral, 
      lexeme,
      translate(lexeme)
    );
  }

  const Token& scan_numeral() {
    auto ch = source_.next();
    while(!std::iswspace(ch) && is_numeral(ch) && !source_.is_eos()) {
      ch = source_.next();
    }

    auto lexeme = get_lexeme();
    auto literal = std::stoll(lexeme);
    return Token(
      Numeral,
      lexeme,
      literal
    );
  }

  std::string get_lexeme() {
    return source_.to_string().substr(start_, source_.position());
  }

  bool is_roman_numeral(const char& ch) {
    switch (std::tolower(ch)) {
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

public:
  Scanner() {}
  Scanner(std::string source) {
    source_ = CharStream(source);
  }

  ~Scanner() {
  }

  Array<Token>& scan() {
    Array<Token> tokens;
    auto t = token();
    while (t.type != EndOfFile) {
      // Add token to array
      tokens.push(t);
      t = token();
    }
    
    return tokens;
  }
};


#endif