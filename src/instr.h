/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  instr.h                                                          *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#pragma once

#include <vector>
#include <capstone.h>

class Instr {
public:
    Instr(cs_insn *insn) : insn_(insn) {}

    uint64_t address() const { return insn_->address; }
    uint16_t size() const { return insn_->size; }
    unsigned int id() const { return insn_->id; }
    const uint8_t *bytes() const { return insn_->bytes; }
    const char *mnemonic() const { return insn_->mnemonic; }
    const char *operands() const { return insn_->op_str; }

    cs_detail *detail() const { return insn_->detail; }

    inline std::vector<uint8_t> read_regs() const {
        auto detail = insn_->detail;
        auto vec = std::vector<uint8_t>();
        vec.assign(detail->regs_read, detail->regs_read + detail->regs_read_count);

        return vec;
    }

    inline std::vector<uint8_t> write_regs() const {
        auto detail = insn_->detail;
        auto vec = std::vector<uint8_t>();
        vec.assign(detail->regs_write, detail->regs_write + detail->regs_write_count);

        return vec;
    }
private:
    cs_insn *insn_;
};
