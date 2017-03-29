/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  disassembler.cc                                                  *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "disassembler.h"


Disassembler::Disassembler(uintptr_t addr, const uint8_t *code, size_t size, Arch arch, Mode mode)
: setup_(false), address_(addr), code_(code), size_(size), arch_(arch), mode_(mode) {}

Disassembler::~Disassembler() {
    cs_close(&handle_);
}

int Disassembler::SetupDisassembler() {
    setup_ = true;
    int res = cs_open((cs_arch)arch_, (cs_mode)mode_, &handle_);
    cs_option(handle_, CS_OPT_DETAIL, CS_OPT_ON);
    return res;
}

Instr Disassembler::Disassemble(size_t off) {
    cs_insn *insn;
    cs_disasm(handle_, code_ + off, size_, address_ + off, 0, &insn);

    return Instr(insn);
}

Instr Disassembler::DisassembleNext(Instr &current) {
  size_t offset = current.address() - address_;
  offset += current.size();
  return Disassemble(offset);
}

void Disassembler::DisassembleIter(IterCallback cb) {
  size_t size = size_;
  const uint8_t *code = code_;
  uint64_t address = address_;
  cs_insn *insn = cs_malloc(handle_);

  while(cs_disasm_iter(handle_, &code, &size, &address, insn)) {
    if (cb) {
      if (!cb(Instr(insn))) {
        break;
      }
    }
  }

  cs_free(insn, 1);
}
