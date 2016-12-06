#include "stdafx.h"
#include "FileStream.h"

FileStream::FileStream(std::string path) : Stream(path), stream_(path) {}

int8_t FileStream::ReadChar() {
	int8_t c;
	ReadBuf(&c, sizeof(c));
	return c;
}

uint8_t FileStream::ReadUChar() {
	uint8_t c;
	ReadBuf(&c, sizeof(c));
	return c;
}

int16_t FileStream::ReadShort() {
	short s;
	ReadBuf(&s, sizeof(s));
	return s;
}

uint16_t FileStream::ReadUShort() {
	int16_t s;
	ReadBuf(&s, sizeof(s));
	return s;
}

int32_t FileStream::ReadInt() {
	int32_t i;
	ReadBuf(&i, sizeof(i));
	return i;
}

uint32_t FileStream::ReadUInt() {
	uint32_t i;
	ReadBuf(&i, sizeof(i));
	return i;
}

int64_t FileStream::ReadLong() {
	int64_t l;
	ReadBuf(&l, sizeof(l));
	return l;
}

uint64_t FileStream::ReadULong() {
	uint64_t l;
	ReadBuf(&l, sizeof(l));
	return l;
}

void FileStream::ReadBuf(void* buf, size_t size) {
	if (stream_.is_open()) {
		stream_.read(reinterpret_cast<char*>(buf), size);
	}
}

int8_t FileStream::PeekChar() {
	int8_t c;
	PeekBuf(&c, sizeof(c));
	return c;
}

uint8_t FileStream::PeekUChar() {
	uint8_t c;
	PeekBuf(&c, sizeof(c));
	return c;
}

int16_t FileStream::PeekShort() {
	short s;
	PeekBuf(&s, sizeof(s));
	return s;
}

uint16_t FileStream::PeekUShort() {
	int16_t s;
	PeekBuf(&s, sizeof(s));
	return s;
}

int32_t FileStream::PeekInt() {
	int32_t i;
	PeekBuf(&i, sizeof(i));
	return i;
}

uint32_t FileStream::PeekUInt() {
	uint32_t i;
	PeekBuf(&i, sizeof(i));
	return i;
}

int64_t FileStream::PeekLong() {
	int64_t l;
	PeekBuf(&l, sizeof(l));
	return l;
}

uint64_t FileStream::PeekULong() {
	uint64_t l;
	PeekBuf(&l, sizeof(l));
	return l;
}

void FileStream::PeekBuf(void* buf, size_t size) {
	size_t offset = Offset();
	if (stream_.is_open()) {
		stream_.read(reinterpret_cast<char*>(buf), size);
	}
	SetOffset(offset);
}

void FileStream::WriteChar(int8_t c) {
	WriteBuf(&c, sizeof(c));
}

void FileStream::WriteUChar(uint8_t c) {
	WriteBuf(&c, sizeof(c));
}

void FileStream::WriteShort(int16_t s) {
	WriteBuf(&s, sizeof(s));
}

void FileStream::WriteUShort(uint16_t s) {
	WriteBuf(&s, sizeof(s));
}

void FileStream::WriteInt(int32_t i) {
	WriteBuf(&i, sizeof(i));
}

void FileStream::WriteUInt(uint32_t i) {
	WriteBuf(&i, sizeof(i));
}

void FileStream::WriteLong(int64_t l) {
	WriteBuf(&l, sizeof(l));
}

void FileStream::WriteULong(uint64_t l) {
	WriteBuf(&l, sizeof(l));
}

void FileStream::WriteBuf(void* buf, size_t size) {
	if (stream_.is_open()) {
		stream_.write(reinterpret_cast<char*>(buf), size);
		stream_.flush();  // needed?
	}
}

size_t FileStream::Offset() {
	return stream_.tellp();
}

void FileStream::SetOffset(size_t offset) {
	stream_.seekp(offset);
}