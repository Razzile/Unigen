#pragma once
#include <stdint.h>
#include <string>
class Stream {
public:
  Stream(std::string path) : path_(path) {}
  virtual int8_t ReadChar() = 0;
  virtual uint8_t ReadUChar() = 0;
  virtual int16_t ReadShort() = 0;
  virtual uint16_t ReadUShort() = 0;
  virtual int32_t ReadInt() = 0;
  virtual uint32_t ReadUInt() = 0;
  virtual int64_t ReadLong() = 0;
  virtual uint64_t ReadULong() = 0;
  virtual void ReadBuf(void *buf, size_t size) = 0;

  virtual int8_t PeekChar() = 0;
  virtual uint8_t PeekUChar() = 0;
  virtual int16_t PeekShort() = 0;
  virtual uint16_t PeekUShort() = 0;
  virtual int32_t PeekInt() = 0;
  virtual uint32_t PeekUInt() = 0;
  virtual int64_t PeekLong() = 0;
  virtual uint64_t PeekULong() = 0;
  virtual void PeekBuf(void *buf, size_t size) = 0;

  virtual void WriteChar(int8_t c) = 0;
  virtual void WriteUChar(uint8_t c) = 0;
  virtual void WriteShort(int16_t s) = 0;
  virtual void WriteUShort(uint16_t s) = 0;
  virtual void WriteInt(int32_t i) = 0;
  virtual void WriteUInt(uint32_t i) = 0;
  virtual void WriteLong(int64_t l) = 0;
  virtual void WriteULong(uint64_t l) = 0;
  virtual void WriteBuf(void *buf, size_t size) = 0;

  virtual size_t Find(char pattern[], size_t size) = 0;

  virtual size_t offset() = 0;
  virtual void set_offset(size_t offset) = 0;

  template <typename T> T Read(size_t off = 0);
  template <typename T> T Peek(size_t off = 0);
  template <typename T> void Write(T val, size_t off = 0);

protected:
  std::string path_;
};

template <typename T> T Stream::Read(size_t off) {
  set_offset(off);
  T temp;
  ReadBuf(&temp, sizeof(T));
  return temp;
}

template <typename T> T Stream::Peek(size_t off) {
  size_t orig = offset();
  set_offset(off);
  T temp;
  PeekBuf(&temp, sizeof(T));
  set_offset(orig);
  return temp;
}

template <typename T> void Stream::Write(T val, size_t off) {
  set_offset(off);
  WriteBuf(&val, sizeof(T));
}
