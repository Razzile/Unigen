/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  assembly_crawler_arm.cc                                          *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "assembly_crawler_arm.h"

namespace arm {

bool AssemblyCrawler::Crawl(uintptr_t start, char *code,
  uintptr_t *meta_reg_out, uintptr_t *code_reg_out) {
  Mode mode;
  bool thumb = start & 0x1;
  if (thumb) {
    mode = Mode::ModeTHUMB;
  } else {
    mode = Mode::ModeARM;
  }
  uintptr_t real_start = start & ~0x1;
  Disassembler ds(real_start, (const uint8_t *)code, 32, Arch::ARM, mode);
  ds.SetupDisassembler();
  ds.DisassembleIter([&](Instr instr) -> bool {

  });

}

}
