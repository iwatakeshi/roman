#include <iostream>
#include <string>

#include "src/scanner.hpp"
#include "src/parser.hpp"
#include "src/array.hpp"
#include "src/roman.hpp"

#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  std::vector<std::string> arguments(argv + 1, argv + argc);

  string input;
  cout << "> ";
  Scanner scanner;
  Parser parser;

  while(std::getline(cin, input)) {
    if (input == "exit") {
      break;
    }
    scanner = Scanner(input);
    auto tokens = scanner.scan();
    parser = Parser(tokens);

    if (arguments.size() > 0 && (arguments[0] == "-r" || arguments[0] == "--roman")) {
      cout << roman(parser.parse()) << endl;
    } else {
      cout << parser.parse() << endl;
    }

    // auto token_stream = scanner.scan();
    // cout << token_stream << endl;
    // for(auto i = 0; i < token_stream.length(); i++) {
    //   auto token = token_stream[i];
    //   if (token.type() == Error) {
    //     cout << token.lexeme() << endl;
    //     break;
    //   }

    //   if (token.type() == Operator) {
    //     cout << token.lexeme() << endl;
    //     continue;
    //   }

    //   if (token.type() == EndOfFile) {
    //     break;
    //   }

    //   cout << token.literal() << endl;
    // }

    cout << "> ";
  }
  
  return 0;
}