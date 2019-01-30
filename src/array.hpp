#ifndef ARRAY_H
#define ARRAY_H
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <stdexcept>

using std::function;
using std::ostream;
using std::string;
using std::is_fundamental;

template <class T>
class Array {
private:
  T * array_ = nullptr;
  uint64_t offset_ = 0;
  uint64_t size_ = 0;
  uint64_t length_ = 0;

public:
  Array() {}
  Array(uint64_t size) {
    array_ = new T[size];
  }

  ~Array() {
    delete [] array_;
    offset_ = 0;
    size_ = 0;
    length_ = 0;
  }

  /**
   * Return the value at the specified index.
   */
  T& operator [] (uint64_t const& index) {
    if (index < 0 || index >= length()) {
      throw std::out_of_range("Index is out of bounds");
    }

    return array_[offset_ + index];
  }

  /**
   * Return an array its value(s) n times.
   */
  Array<T> operator * (uint64_t const & right) {
    Array<T> temp;
    for (auto i = 0; i < right; i++) {
     for (auto j = this->offset_; j < this->length_; j++) {
       temp.push(this->array_[j]);
     }
    }

    return temp;
  }

  Array<T>& operator = (Array<T> const& right) {
    if (size_ < right.size_) {
      reserve(right.size_, false);
    }

    size_ = right.size_;
    offset_ = 0;
    length_ = right.length_ + right.offset_;

    for (auto i = 0; i < size_; i++) {
      array_[i] = right.array_[right.offset_ + i];
    }

    return *this;
  }

  /**
   * Return a concantenated array.
   */
  Array<T> operator + (Array<T> const& right) {
    Array<T> temp;
    for (auto i = right.offset_; i < right.length_; i++) {
      temp.push(right.array_[i]);
    }
    return temp;
  }

  /**
   * Assign a concantenated array.
   */
  Array<T>& operator += (Array<T> const& right) {
    for (auto i = right.offset_; i < right.length_; i++) {
      this->push(right.array_[i]);
    }

    return *this;
  }

  /**
   * Assign an array with its value(s) n times.
   */
  Array<T>& operator *= (uint64_t const& right) {
    T * temp = new T[(length_ - offset_) * right];
    auto index = 0;
    for (auto i = 0; i < right; i++) {
     for (auto j = offset_; j < length_; j++) {
       temp[index] = array_[j];
       index += 1;
     }
    }

    delete [] array_;
    array_ = temp;
    size_ = (length_ - offset_) * right;
    length_ = size_;
    offset_ = 0;

    return *this;
  }

  /**
   * Return an output stream representation of the array.
   */
  friend ostream& operator << (ostream& os, const Array<T>& array) {
    string seperator = ", ";
    string result = "";
    auto length = array.length_;
    for(auto i = array.offset_; i < length; i++) {
      if (i == length - 1) {
        seperator = "";
      }

      // Determine whether we have a primitive type.
      if (is_fundamental<T>::value) {
        result += (std::to_string(array.array_[i]) + seperator);
      } else {
        result += (typeid(array).name() + seperator);
      }
    }
    os  << "[" << result << "]";
    return os;
  }

  void unshift(T const& value) {
    // Determine if the array is full
    auto size = (size_ == 0) ? 1 : 
      is_full() ? 2 * size_ : length() + 1;
    T * array = new T[size];
    
    array[0] = value;

    for(auto i = 1; i <= length(); i++) {
      array[i] = array_[offset_ + (i - 1)];
    }

    delete [] array_;
    array_ = array;

    length_ += 1;
    size_ = size;
  }

  T& shift() {
    if (is_empty()) {
      throw std::out_of_range("Array is empty"); 
    }

    T& element = array_[offset_];
    offset_ += 1;

    return element;
  }

  void push(T const& value) {
    // Determine if the array is full
    if (is_full()) {
      auto size = size_ == 0 ? 1 : 2 * size_;
      auto copy = size_ == 0 ? false : true;
      reserve(size, copy);
    }

    array_[length()] = value;
    length_ += 1;
  }

  T& pop() {
    if (is_empty()) {
      throw std::out_of_range("Array is empty");
    }

    T& element = array_[length() - 1];
    length_ -= 1;

    return element;
  }

  /**
   * Iterate through each value in the array. 
   */
  void for_each(function<void (T)>const& lambda) {
    for(uint64_t i = 0; i < length(); i++) {
      lambda(array_[offset_ + i]);
    }
  }

  /**
   * Iterate through each value in the array. 
   */
  void for_each(function<void (T, uint64_t)>const& lambda) {
    for(uint64_t i = 0; i < length(); i++) {
      lambda(array_[offset_ + i], i);
    }
  }

  /**
   * Filter the array based on a condition.
   */
  Array<T> filter(function<bool (T)> const& lambda) {
    Array<T> temp;
    for(uint64_t i = 0; i < length(); i++) {
      if (lambda(array_[offset_ + i])) {
        temp.push(array_[offset_ + i]);
      }
    }
    return temp;
  }

  /**
   * Filter the array based on a condition.
   */
  Array<T> filter(function<bool (T, uint64_t)> const& lambda) {
    Array<T> temp;
    for(uint64_t i = 0; i < length(); i++) {
      if (lambda(array_[offset_ + i], i)) {
        temp.push(array_[offset_ + i]);
      }
    }
    return temp;
  }

  /**
   * Map the values in the array and return a new array.
   */
  Array<T> map(function<T (T)> const& lambda) {
    Array<T> temp;
    for(uint64_t i = 0; i < length(); i++) {
      temp.push(lambda(array_[offset_ + i]));
    }
    return temp;
  }

  /**
   * Map the values in the array and return a new array.
   */
  Array<T> map(function<T (T, uint64_t)> const& lambda) {
    Array<T> temp;
    for(uint64_t i = 0; i < length(); i++) {
      temp.push(lambda(array_[offset_ + i], i));
    }
    return temp;
  }

  /**
   * Map the values in the array and return a new array.
   */
  template <typename U>
  Array<U> map(function<U (T)> const& lambda) {
    Array<U> temp;
    for(uint64_t i = 0; i < length(); i++) {
      temp.push(lambda(array_[offset_ + i]));
    }
    return temp;
  }

  /**
   * Map the values in the array and return a new array.
   */
  template <typename U>
  Array<U> map(function<U (T, uint64_t)> const& lambda) {
    Array<U> temp;
    for(uint64_t i = 0; i < length(); i++) {
      temp.push(lambda(array_[offset_ + i], i));
    }
    return temp;
  }

  string join() {
    return this->join(",");
  }

  string join(const string& seperator) {
    string result = "";
    this->for_each([&] (T x) {
      result += (((string) x) + seperator);
    });
    return result;
  }

  void reserve(uint64_t size) {
    reserve(size, false);
  }

  void reserve(uint64_t size, bool copy) {
    T * array = new T[size];

    if (copy) {
      for(auto i = 0; i < size_; i++) {
        array[i] = array_[offset_ + i];
      }
    }

    delete [] array_;

    size_ = size;
    array_ = array;
  }

  uint64_t length() {
    return length_ - offset_;
  }

  bool is_empty() {
    return length() == 0;
  }

  bool is_full() {
    return length() == size_;
  }
};


#endif