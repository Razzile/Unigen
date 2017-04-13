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

enum class BinaryType {
    MachO,
    FatMachO,
    ELF,
    PE,
    UNKNOWN
};

enum class SectionType {
    INVALID,
    TEXT,
    DATA,
    UNKNOWN
};

class Binary {
public:
    static BinaryType Type(MemoryStream *stream);

    Binary(std::string file) : stream_(file), type_(BinaryType::UNKNOWN) {}
    Binary(MemoryStream &stream) : stream_(stream), type_(BinaryType::UNKNOWN) {}

    MemoryStream &stream() { return stream_; }

    virtual uintptr_t FindMetadataRegistration() = 0;
    virtual uintptr_t FindCodeRegistration() = 0;

    virtual uintptr_t PhysToVirt(uintptr_t addr) = 0;
    virtual uintptr_t VirtToPhys(uintptr_t addr) = 0;

    virtual SectionType SectionTypeForAddress(uintptr_t addr) = 0;
protected:
    MemoryStream stream_;
    BinaryType type_;
};

}
