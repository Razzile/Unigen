/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  memory_stream.cc                                                 *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#include "MemoryStream.h"
#include <assert.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

inline static void *_memmem(const void *l, size_t l_len, const void *s,
                            size_t s_len) {
  char *cur, *last;
  const char *cl = (const char *)l;
  const char *cs = (const char *)s;

  /* we need something to compare */
  if (l_len == 0 || s_len == 0)
    return NULL;

  /* "s" must be smaller or equal to "l" */
  if (l_len < s_len)
    return NULL;

  /* special case where s_len == 1 */
  if (s_len == 1)
    return (void *)memchr(l, (int)*cs, l_len);

  /* the last position where its possible to find "s" in "l" */
  last = (char *)cl + l_len - s_len;

  for (cur = (char *)cl; cur <= last; cur++)
    if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
      return cur;

  return NULL;
}

// TODO: store handles in object for use in destructior
MemoryStream::MemoryStream(std::string path) : Stream(path), pos_(0) {
#ifdef _WIN32
  HANDLE hfile =
      CreateFileA(path_.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                  OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
  size_ = GetFileSize(hfile, 0);
  assert(hfile != INVALID_HANDLE_VALUE);
  //
  HANDLE hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE, 0, 0, 0);
  assert(hmap != NULL);
  //
  base_ = (char *)MapViewOfFile(hmap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
#else
  struct stat sb;
  int fd = open(path.data(), O_RDWR);
  fstat(fd, &sb);
  size_ = sb.st_size;

  base_ =
      (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif
}

int8_t MemoryStream::ReadChar() {
  int8_t c;
  ReadBuf(&c, sizeof(c));
  return c;
}

uint8_t MemoryStream::ReadUChar() {
  uint8_t c;
  ReadBuf(&c, sizeof(c));
  return c;
}

int16_t MemoryStream::ReadShort() {
  short s;
  ReadBuf(&s, sizeof(s));
  return s;
}

uint16_t MemoryStream::ReadUShort() {
  int16_t s;
  ReadBuf(&s, sizeof(s));
  return s;
}

int32_t MemoryStream::ReadInt() {
  int32_t i;
  ReadBuf(&i, sizeof(i));
  return i;
}

uint32_t MemoryStream::ReadUInt() {
  uint32_t i;
  ReadBuf(&i, sizeof(i));
  return i;
}

int64_t MemoryStream::ReadLong() {
  int64_t l;
  ReadBuf(&l, sizeof(l));
  return l;
}

uint64_t MemoryStream::ReadULong() {
  uint64_t l;
  ReadBuf(&l, sizeof(l));
  return l;
}

void MemoryStream::ReadBuf(void *buf, size_t size) {
  memcpy(buf, base_ + pos_, size);
  pos_ += size;
}

int8_t MemoryStream::PeekChar() {
  int8_t c;
  PeekBuf(&c, sizeof(c));
  return c;
}

uint8_t MemoryStream::PeekUChar() {
  uint8_t c;
  PeekBuf(&c, sizeof(c));
  return c;
}

int16_t MemoryStream::PeekShort() {
  short s;
  PeekBuf(&s, sizeof(s));
  return s;
}

uint16_t MemoryStream::PeekUShort() {
  int16_t s;
  PeekBuf(&s, sizeof(s));
  return s;
}

int32_t MemoryStream::PeekInt() {
  int32_t i;
  PeekBuf(&i, sizeof(i));
  return i;
}

uint32_t MemoryStream::PeekUInt() {
  uint32_t i;
  PeekBuf(&i, sizeof(i));
  return i;
}

int64_t MemoryStream::PeekLong() {
  int64_t l;
  PeekBuf(&l, sizeof(l));
  return l;
}

uint64_t MemoryStream::PeekULong() {
  uint64_t l;
  PeekBuf(&l, sizeof(l));
  return l;
}

void MemoryStream::PeekBuf(void *buf, size_t size) {
  memcpy(buf, base_ + pos_, size);
}

void MemoryStream::WriteChar(int8_t c) { WriteBuf(&c, sizeof(c)); }

void MemoryStream::WriteUChar(uint8_t c) { WriteBuf(&c, sizeof(c)); }

void MemoryStream::WriteShort(int16_t s) { WriteBuf(&s, sizeof(s)); }

void MemoryStream::WriteUShort(uint16_t s) { WriteBuf(&s, sizeof(s)); }

void MemoryStream::WriteInt(int32_t i) { WriteBuf(&i, sizeof(i)); }

void MemoryStream::WriteUInt(uint32_t i) { WriteBuf(&i, sizeof(i)); }

void MemoryStream::WriteLong(int64_t l) { WriteBuf(&l, sizeof(l)); }

void MemoryStream::WriteULong(uint64_t l) { WriteBuf(&l, sizeof(l)); }

void MemoryStream::WriteBuf(void *buf, size_t size) {
  memcpy(base_ + pos_, buf, size);
  pos_ += size;
}

size_t MemoryStream::Find(char pattern[], size_t size) {
  if (pos_ + size > size_)
    return -1;
  char *pos = (char *)_memmem(base_ + pos_, size_ - pos_, pattern, size);
  if (pos)
    return pos - base_;
  return -1;
}

size_t MemoryStream::offset() { return pos_; }

void MemoryStream::set_offset(size_t offset) { pos_ = offset; }
