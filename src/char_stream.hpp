#ifndef StringStream_H
#define StringStream_H
#include <string>

class CharStream {
private:
  /* data */
  std::string source_ = "";
  uint64_t position_ = 0;
public:
  CharStream() {}
  CharStream(const std::string& source) {
    source_ = source;
  }
  ~CharStream() {
    source_ = "";
  }

  const char previous() {
    if (position_ < 0) {
      throw "position out of bounds.";
    }
    position_--;
    return std::tolower(source_[position_ + 1]);
  }

  const char current() {
    return std::tolower(source_[position_]);
  }
  
  const char next() {
    if (position_ >= source_.length()) {
      throw "position out of bounds.";
    }
    position_++;
    return std::tolower(source_[position_ - 1]);
  }

  const char peek(int64_t by) {
    if ((by + position_) < 0 || (by + position_) >= source_.length()) {
      throw "position out of bounds.";
    }
    return std::tolower(source_[by + position_]);
  }

  bool is_eos() {
    return position_ >= source_.length();
  }

  std::string to_string() {
    return source_;
  }

  std::string range(int start) {
    return range(start, position());
  }

  std::string range(int start, int end) {
    // Implemented my own substring method
    // because std's is buggy(?)
    std::string temp = "";
    for(auto i = start; i < end; i++) {
     temp += std::tolower(source_[i]);
    }
    return temp;
  }

  uint64_t position() {
    return position_;
  }
  
};

#endif