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
#include <capstone.h>
#include <functional>
#include "instr.h"

#if defined(__x86_64__)
#define HOST_ARCH Arch::x86
#define HOST_MODE Mode::Mode64
#elif defined(__i386__)
#define HOST_ARCH Arch::x86
#define HOST_MODE Mode::Mode32
#elif defined(__arm64__)
#define HOST_ARCH Arch::ARM64
#define HOST_MODE Mode::Mode64
#elif defined(__arm__)
#define HOST_ARCH Arch::ARM
#define HOST_MODE Mode::ModeTHUMB
#else
#error building for an invalid arch
#endif

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
    Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch = HOST_ARCH, Mode mode = HOST_MODE);
    ~Disassembler();

    int SetupDisassembler();

    Instr Disassemble();
    void DisassembleIter(IterCallback cb);

    bool setup() const { return setup_; }

    uintptr_t address() const { return address_; }
    void set_address(uintptr_t address) { address_ = address; }

    const uint8_t *code() const { return code_; }

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
