#pragma once

#include <stdint.h>

static inline uint16_t byte_swap(uint16_t value) {
  return ((value >> 8) & 0x00ff) | ((value << 8) & 0xff00);
}

static inline uint32_t byte_swap(uint32_t value) {
  value = ((value >> 8) & 0x00ff00ff) | ((value << 8) & 0xff00ff00);
  value = ((value >> 16) & 0x0000ffff) | ((value << 16) & 0xffff0000);
  return value;
}

static inline uint64_t byte_swap(uint64_t value) {
  value =
      (value & 0x00000000ffffffff) << 32 | (value & 0xffffffff00000000) >> 32;
  value =
      (value & 0x0000ffff0000ffff) << 16 | (value & 0xffff0000ffff0000) >> 16;
  value = (value & 0x00ff00ff00ff00ff) << 8 | (value & 0xff00ff00ff00ff00) >> 8;
  return value;
}

template <typename T> struct endian {
  endian() = default;
  endian(const T &src) : value(byte_swap(src)) {}
  endian(const endian &other) { value = other.value; }
  operator T() const { return byte_swap(value); }

  endian<T> &operator+=(int a) {
    *this = *this + a;
    return *this;
  }
  endian<T> &operator-=(int a) {
    *this = *this - a;
    return *this;
  }
  endian<T> &operator++() {
    *this += 1;
    return *this;
  } // ++a
  endian<T> operator++(int) {
    *this += 1;
    return (*this - 1);
  } // a++
  endian<T> &operator--() {
    *this -= 1;
    return *this;
  } // --a
  endian<T> operator--(int) {
    *this -= 1;
    return (*this + 1);
  } // a--

  T value;
};
