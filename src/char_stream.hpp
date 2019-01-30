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
  CharStream(std::string source) {
    source_ = source;
  }
  ~CharStream() {
    source_ = "";
  }

  const char& previous() {
    if (position_ < 0) {
      throw "position_ out of bounds.";
    }
    return source_[position_--];
  }
  
  const char& next() {
    if (position_ >= source_.length()) {
      throw "position_ out of bounds.";
    }
    return source_[position_++];
  }

  const char& peek(int64_t by) {
    if ((by + position_) < 0 || (by + position_) >= source_.length()) {
      throw "position_ out of bounds.";
    }
    return source_[by + position_];
  }

  bool is_eos() {
    return position_ >= source_.length();
  }

  std::string to_string() {
    return source_;
  }

  uint64_t position() {
    return position_;
  }
  
};

#endif