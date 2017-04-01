/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  binary.h                                                         *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once
#include "memory_stream.h"

namespace base {
// in future binary will hold a mach-o/elf parser obj

enum BinaryType {
    MachO,
    FatMachO,
    ELF,
    PE,
    UNKNOWN
};

class Binary {
public:
    static BinaryType Type(MemoryStream *stream);

    Binary(std::string file) : stream_(file), type_(UNKNOWN) {}
    Binary(MemoryStream &stream) : stream_(stream), type_(UNKNOWN) {}

    MemoryStream &stream() { return stream_; }

    virtual uintptr_t FindMetadataRegistration() = 0;
    virtual uintptr_t FindCodeRegistration() = 0;

    virtual uintptr_t ConvertVirtualAddress(uintptr_t addr) = 0;
    virtual bool IsText(uintptr_t addr) = 0;
protected:
    MemoryStream stream_;
    BinaryType type_;
};

}
