#ifndef TRANSLATE_H
#define TRANSLATE_H
#include <string>
#include <map>
#include <iostream>
#include <cmath>
#include "array.hpp"


using std::cout;
using std::endl;
using std::string;
using std::map;

int64_t roman(std::string lexeme) {
  map<char, int64_t> roman_map = {
    { 'i', 1 },
    { 'v', 5 },
    { 'x', 10 },
    { 'l', 50 },
    { 'c', 100 },
    { 'd', 500 },
    { 'm', 1000 }
  };

  auto literal = 0;
  int64_t position = 0;

  auto current = [&] () {
    return lexeme[position];
  };

  auto next = [&]() {
    return lexeme[position++];
  };

  auto peek = [&] (int by) {
    return lexeme[position + by];
  };
  
  auto consume = [&] (char ch) -> void {
   literal += roman_map.at(ch);
  };

  auto consume_other = [&] (char ch, char other) -> void {
    literal += std::fabs((roman_map.at(other) - roman_map.at(ch)));
  };

  while (position < lexeme.length()) {
    char ch = current();
    switch (ch) {
      case 'i':
        next();
        if (current() == 'v' || current() == 'x') {
          char other = next();
          consume_other(ch, other);
        } else {
          consume(ch);
        }
        break;
      case 'v':
        next();
        consume(ch);
        break;
      case 'x':
        next();
        if (current() == 'l' || current() == 'c') {
          char other = next();
          consume_other(ch, other);
        } else  {
          consume(ch);
        }
        break;
      case 'c':
        next();
        if (current() == 'd' || current() == 'm') {
          char other = next();
          consume_other(ch, other);
        } else {
          consume(ch);
        }
        break;
      case 'l':
      case 'd':
      case 'm':
        next();
        consume(ch);
        break;
      default:
        string error = "Invalid roman numeral '";
        error.append(1, ch);
        error += "'";
        throw error;
        break;
    }
    // ch = next();
  }
  return literal;
}

string roman(int64_t literal) {
  // if (std::to_string(literal).length() > 4) {
  //   string error = "Number greater than 1000 is not supported.";
  //   throw error;
  // }
  map<int64_t, string> roman_map = {
    { 1,    "i" },
    { 5,    "v" },
    { 10,   "x" },
    { 50,   "l" },
    { 100,  "c" },
    { 500,  "d" },
    { 1000, "m" }
  };

  auto place_value = [](uint length) {
    map<uint, uint> places = {
      { 1, 1 }, 
      { 2, 10 }, 
      { 3, 100 },
      { 4, 1000 },
      { 5, 10000 }
    };
    return places.at(length);
  };

  auto convert = [&](uint number) {
    try {
      return roman_map.at(number);
    } catch (...) {
      auto index = place_value(std::to_string(number).length());
      auto pivot = number / index;
      // cout << "number: " << number << " index: " << index << " " << pivot << endl;
      if (pivot > 5 && pivot < 9) {
        pivot -= 5;
        return roman_map.at(index * 5) + string(pivot, roman_map.at(index).c_str()[0]);
      }

      if (pivot == 4) {
        return roman_map.at(index) + roman_map.at(index * 5);
      }

      if (pivot == 9) {
        return roman_map.at(index) + roman_map.at(index * 10);
      }

      return string(pivot, roman_map.at(index).c_str()[0]);
    }
  };

  
  array<int> num_chars;
  for(auto ch : std::to_string(literal)) {
    num_chars.unshift(ch - '0');
  }
  
  auto a = num_chars.map([&](auto n, auto i) {
    // cout << n * place_value(i + 1) << endl;
    return n * place_value(i + 1);
  });

  auto b = a.map<string>([&](auto n) -> string {
    return convert(n);
  });

  return b.reverse().join("");
}

#endif