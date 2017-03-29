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
#include <capstone/capstone.h>

class Instr {
public:
    Instr(cs_insn *insn) : insn_(insn) {}

    uint64_t address() const { return insn_->address; }
    uint16_t size() const { return insn_->size; }
    unsigned int id() const { return insn_->id; }
    const uint8_t *bytes() const { return insn_->bytes; }
    const char *mnemonic() const { return insn_->mnemonic; }
    const char *operands() const { return insn_->op_str; }

    cs_insn *insn() const { return insn_; }
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

    inline bool ReadsReg(csh handle, uint32_t reg) const {
      cs_regs regs_read, regs_write;
      uint8_t read_count, write_count;
      cs_regs_access(handle, insn_, regs_read, &read_count, regs_write, &write_count);
      if (read_count > 0) {
        for (int i = 0; i < read_count; i++) {
          if (regs_read[i] == reg) {
            return true;
          }
        }
      }
      return false;
    }

    inline bool WritesReg(csh handle, uint32_t reg) const {
      cs_regs regs_read, regs_write;
      uint8_t read_count, write_count;
      cs_regs_access(handle, insn_, regs_read, &read_count, regs_write, &write_count);
      if (write_count > 0) {
        for (int i = 0; i < write_count; i++) {
          if (regs_write[i] == reg) {
            return true;
          }
        }
      }
      return false;
    }

private:
    cs_insn *insn_;
};
