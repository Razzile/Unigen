#pragma once
#include "Stream.h"

class MemoryStream : public Stream {
public:
  MemoryStream(std::string path);

  char *base() const { return base_; }
  char *pointer() const { return base_ + pos_; }
  size_t size() const { return size_; }
  
  char *MapPtr(void *ptr) { return base_ + (uintptr_t)ptr; }

  int8_t ReadChar() override;
  uint8_t ReadUChar() override;
  int16_t ReadShort() override;
  uint16_t ReadUShort() override;
  int32_t ReadInt() override;
  uint32_t ReadUInt() override;
  int64_t ReadLong() override;
  uint64_t ReadULong() override;
  void ReadBuf(void *buf, size_t size) override;

  int8_t PeekChar() override;
  uint8_t PeekUChar() override;
  int16_t PeekShort() override;
  uint16_t PeekUShort() override;
  int32_t PeekInt() override;
  uint32_t PeekUInt() override;
  int64_t PeekLong() override;
  uint64_t PeekULong() override;
  void PeekBuf(void *buf, size_t size) override;

  void WriteChar(int8_t c) override;
  void WriteUChar(uint8_t c) override;
  void WriteShort(int16_t s) override;
  void WriteUShort(uint16_t s) override;
  void WriteInt(int32_t i) override;
  void WriteUInt(uint32_t i) override;
  void WriteLong(int64_t l) override;
  void WriteULong(uint64_t l) override;
  void WriteBuf(void *buf, size_t size) override;

  size_t Find(char pattern[], size_t size) override;

  size_t offset() override;
  void set_offset(size_t offset) override;

  template <typename T> T *View(size_t offset);

private:
  char *base_;
  size_t size_;
  size_t pos_;
};
 
template <typename T>
T *MemoryStream::View(size_t offset) {
  char *temp = (base_ + offset);
  return (T*)temp;
}

