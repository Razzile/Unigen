/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  binary_container.h                                               *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once

#include <LIEF/LIEF.hpp>
#include "memory_stream.h"
#include "utils.h"

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

/**
 * Convenience wrapper for a LIEF binary
 */
class BinaryContainer {
public:
    static BinaryType Type(std::string file);

    BinaryContainer(std::string file);
    ~BinaryContainer();

    MemoryStream &stream() { return stream_; }

    virtual address_t FindMetadataRegistration() = 0;
    virtual address_t FindCodeRegistration() = 0;

    virtual address_t PhysToVirt(address_t addr) = 0;
    virtual address_t VirtToPhys(address_t addr) = 0;

    virtual SectionType SectionTypeForAddress(address_t addr) = 0;
private:
    SectionType SectionTypeForAddressELF(address_t addr);
    SectionType SectionTypeForAddressPE(address_t addr);
    SectionType SectionTypeForAddressMachO(address_t addr);

    LIEF::Binary *bin_;
    MemoryStream stream_;
    BinaryType type_;
};

}
