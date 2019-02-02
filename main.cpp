#include <iostream>
#include <string>

#include "src/scanner.hpp"
#include "src/array.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {

  string input;
  cout << "> ";
  Scanner * scanner;
  while(std::getline(cin, input)) {
    if (input == "exit") {
      break;
    }
    scanner = new Scanner(input);
    auto token_stream = scanner->scan();
    cout << token_stream << endl;
    for(auto i = 0; i < token_stream.length(); i++) {
      auto token = token_stream[i];
      if (token.type() == Error) {
        cout << token.lexeme() << endl;
        break;
      }

      if (token.type() == Operator) {
        cout << token.lexeme() << endl;
        continue;
      }

      if (token.type() == EndOfFile) {
        break;
      }

      cout << token.literal() << endl;
    }

    cout << "> ";
  }

  delete scanner;
  
  return 0;
}