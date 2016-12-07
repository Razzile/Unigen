#include "stdafx.h"
#include "MemoryStream.h"
#include <assert.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#endif

MemoryStream::MemoryStream(std::string path) : Stream(path), pos_(0) {
#ifdef _WIN32
	HANDLE hfile = CreateFileA(path_.data(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	size_ = GetFileSize(hfile, 0);
	assert(hfile != INVALID_HANDLE_VALUE);
	//
	HANDLE hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE | SEC_RESERVE, 0, 0, 0);
	assert(hmap != NULL);
	//
	base_ = (char *)MapViewOfFile(hmap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
#else

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

void MemoryStream::ReadBuf(void* buf, size_t size) {
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

void MemoryStream::PeekBuf(void* buf, size_t size) {
	memcpy(buf, base_ + pos_, size);
}

void MemoryStream::WriteChar(int8_t c) {
	WriteBuf(&c, sizeof(c));
}

void MemoryStream::WriteUChar(uint8_t c) {
	WriteBuf(&c, sizeof(c));
}

void MemoryStream::WriteShort(int16_t s) {
	WriteBuf(&s, sizeof(s));
}

void MemoryStream::WriteUShort(uint16_t s) {
	WriteBuf(&s, sizeof(s));
}

void MemoryStream::WriteInt(int32_t i) {
	WriteBuf(&i, sizeof(i));
}

void MemoryStream::WriteUInt(uint32_t i) {
	WriteBuf(&i, sizeof(i));
}

void MemoryStream::WriteLong(int64_t l) {
	WriteBuf(&l, sizeof(l));
}

void MemoryStream::WriteULong(uint64_t l) {
	WriteBuf(&l, sizeof(l));
}

void MemoryStream::WriteBuf(void* buf, size_t size) {
	memcpy(base_ + pos_, buf, size);
	pos_ += size;
}

size_t MemoryStream::offset() {
	return pos_;
}

void MemoryStream::set_offset(size_t offset) {
	pos_ = offset;
}