/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  disassembler.h                                                   *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#pragma once

#include <sys/types.h>
#include <capstone/capstone.h>
#include <functional>
#include "instr.h"

using IterCallback = std::function<bool(Instr)>;

enum class Arch {
    x86 = CS_ARCH_X86,
    x64 = CS_ARCH_X86,
    ARM = CS_ARCH_ARM,
    ARM64 = CS_ARCH_ARM64
};

enum class Mode {
    Mode32 = CS_MODE_32,
    Mode64 = CS_MODE_64,
    ModeARM = CS_MODE_ARM,
    ModeTHUMB = CS_MODE_THUMB
};

class Disassembler {
public:
    Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch, Mode mode);
    ~Disassembler();

    int SetupDisassembler();

    Instr Disassemble(size_t off = 0);
    Instr DisassembleNext(Instr &current);
    void DisassembleIter(IterCallback cb);

    bool setup() const { return setup_; }

    uintptr_t address() const { return address_; }
    void set_address(uintptr_t address) { address_ = address; }

    const uint8_t *code() const { return code_; }
    void set_code(const uint8_t *code) { code_ = code; }

    Arch arch() const { return arch_; }
    Mode mode() const { return mode_; }

    csh handle() { return handle_; }
protected:
    bool setup_;
    uintptr_t address_;
    const uint8_t *code_;
    size_t size_;
    Arch arch_;
    Mode mode_;
    csh handle_;
};
