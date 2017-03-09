#pragma once
#include "memory_stream.h"

namespace base {
// in future binary will hold a mach-o/elf parser obj

enum BinaryType {
    MachO,
    ELF,
    UNKNOWN
};

class Binary {
public:
    Binary(std::string file) : stream_(file), type_(UNKNOWN) {}
    Binary(MemoryStream &stream) : stream_(stream), type_(UNKNOWN) {}
  
    MemoryStream &stream() { return stream_; }

    uintptr_t FindMetadataRegistration();
    uintptr_t FindCodeRegistration();
protected:
    MemoryStream stream_;
    BinaryType type_;
};

}