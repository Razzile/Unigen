/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  macho_binary.h                                                   *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#pragma once

#include "binary.h"

namespace macho {

// represents a MachO slice
class Binary : public base::Binary {
public:
  Binary(std::string path, uintptr_t base) : base::Binary(path), base_(base) {}
  Binary(MemoryStream &stream, uintptr_t base) : base::Binary(stream), base_(base) {}

  uintptr_t ConvertVirtualAddress(uintptr_t addr) override;

  uintptr_t FindMetadataRegistration() override;
  uintptr_t FindCodeRegistration() override;

  uintptr_t base() { return base_; }
  uintptr_t set_base(uintptr_t base) { base_ = base; }

private:
  bool IsValid();
  bool Is64Bit();

  uintptr_t FindLoadCommand(uint32_t load_command, bool rebase = true);
  uintptr_t FindSegment(std::string segment, bool rebase = true);
  uintptr_t FindSection(std::string section, bool rebase = true);

  uintptr_t SegmentForAddress(uintptr_t address, bool rebase = true);

  uintptr_t base_;
};

}
