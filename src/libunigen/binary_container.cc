/**
******************************************************************************
* Unigen :  Unity metadata parser for il2cpp games                           *
* File   :  binary_container.cc                                              *
******************************************************************************
* Copyright 2017 Satori. All rights reserved.                                *
******************************************************************************
*/

#include "binary_container.h"

namespace base {

BinaryContainer::BinaryContainer(std::string file) : stream_(file) {
    type_ = BinaryContainer::Type(file);
    bin_ = LIEF::Parser::parse(file);
}

BinaryContainer::~BinaryContainer() {
    delete bin_;
}

BinaryType BinaryContainer::Type(std::string file) {
    if (LIEF::ELF::is_elf(file)) {
        return BinaryType::ELF;
    } else if (LIEF::PE::is_pe(file)) {
        return BinaryType::PE;
    } else if (LIEF::MachO::is_fat(file)) {
        return BinaryType::FatMachO;
    } else if (LIEF::MachO::is_macho(file)) {
        return BinaryType::MachO;
    } else {
        return BinaryType::UNKNOWN;
    }
}

address_t BinaryContainer::PhysToVirt(address_t addr) {
    auto sections = bin_->get_sections();
    for (const LIEF::Section &section : sections) {
        address_t offset = section.offset();
        size_t size = section.size();

        if (offset <= addr && offset + size > addr) {
            address_t slide = addr - offset;
            return section.virtual_address() + slide;
        }
    }
    return 0; // throw exception?
}

address_t BinaryContainer::VirtToPhys(address_t addr) {
    auto sections = bin_->get_sections();
    for (const LIEF::Section &section : sections) {
        address_t address = section.virtual_address();
        size_t size = section.size();

        if (address <= addr && address + size > addr) {
            address_t slide = addr - address;
            return section.offset() + slide;
        }
    }
    return 0; // throw exception?
}

SectionType BinaryContainer::SectionTypeForAddress(address_t addr) {
    switch (type_) {
        case BinaryType::ELF: {
            return SectionTypeForAddressELF(addr);
        }
        case BinaryType::PE: {
            return SectionTypeForAddressPE(addr);
        }
        case BinaryType::MachO: {
            return SectionTypeForAddressMachO(addr);
        }
        default: {
            return SectionType::UNKNOWN;
        }
    }
}

SectionType BinaryContainer::SectionTypeForAddressELF(address_t addr) {
    using LIEF::ELF::SECTION_FLAGS;

    LIEF::ELF::Binary *bin = dynamic_cast<decltype(bin)>(bin_);
    // dynamic_cast should always succeed here but check just in case
    if (bin) {
        for (const LIEF::ELF::Section &sect : bin->get_sections()) {

            auto sect_address = sect.virtual_address();
            auto sect_size = sect.size();

            if (sect_address <= addr && sect_address + sect_size > addr) {
                if (sect.has_flag(SECTION_FLAGS::SHF_EXECINSTR)) {
                    return SectionType::TEXT;
                } else {
                    return SectionType::DATA; //XXX: this is bad!
                }
            }
        }
    }
    return SectionType::UNKNOWN;
}

SectionType BinaryContainer::SectionTypeForAddressPE(address_t addr) {
    using LIEF::PE::SECTION_TYPES;

    LIEF::PE::Binary *bin = dynamic_cast<decltype(bin)>(bin_);
    // dynamic_cast should always succeed here but check just in case
    if (bin) {
        for (LIEF::PE::Section &sect : bin->get_sections()) {
            auto sect_address = sect.virtual_address();
            auto sect_size = sect.size();

            if (sect_address <= addr && sect_address + sect_size > addr) {
                if (sect.is_type(SECTION_TYPES::TEXT)) {
                    return SectionType::TEXT;
                } else if (sect.is_type(SECTION_TYPES::DATA) || sect.is_type(SECTION_TYPES::BSS)) {
                    return SectionType::DATA;
                } else {
                    return SectionType::INVALID;
                }
            }
        }
    }
    return SectionType::UNKNOWN;
}

SectionType BinaryContainer::SectionTypeForAddressMachO(address_t addr) {
    using namespace LIEF::MachO;
    LIEF::MachO::Binary *bin = dynamic_cast<decltype(bin)>(bin_);
    // dynamic_cast should always succeed here but check just in case
    if (bin) {
        LIEF::MachO::SegmentCommand &seg = bin->segment_from_offset(addr);
        auto prot = seg.init_protection();
        if (prot & VM_PROT_EXECUTE) {
            return SectionType::TEXT;
        } else if (prot & VM_PROT_READ) {
            return SectionType::DATA;
        } else {
            return SectionType::INVALID;
        }
    }
    return SectionType::UNKNOWN;
}

}
