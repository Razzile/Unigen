/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  assembly_crawler_arm64.cc                                        *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "assembly_crawler_arm64.h"

namespace arm64 {

bool AssemblyCrawler::Crawl(uintptr_t start, char *code,
  uintptr_t *meta_reg_out, uintptr_t *code_reg_out) {
  Disassembler ds(start, (const uint8_t *)code, 32, Arch::ARM64, Mode::ModeARM);
  ds.SetupDisassembler();
  ds.DisassembleIter([&](Instr instr) -> bool {
    uint32_t id = instr.id();
    cs_detail *detail = instr.detail();
    csh handle = ds.handle();

    switch (id) {
      case ARM64_INS_BL: {
        cs_arm64_op op = detail->arm64.operands[0];
        uintptr_t target = op.imm;
        printf("%llx\n", target);
      }
    }

    // for (int n = 0; n < detail->arm64.op_count; n++) {
    //   cs_arm64_op *op = &(detail->arm64.operands[n]);
    //  switch(op->type) {
    //     case ARM64_OP_REG:
    //      printf("\t\toperands[%u].type: REG = %s\n", n, cs_reg_name(handle, op->reg));
    //      continue;
    //    case ARM64_OP_IMM:
    //      printf("\t\toperands[%u].type: IMM = 0x%x\n", n, op->imm);
    //      continue;
    //    case ARM64_OP_FP:
    //      printf("\t\toperands[%u].type: FP = %f\n", n, op->fp);
    //      continue;
    //    case ARM64_OP_MEM:
    //      printf("\t\toperands[%u].type: MEM\n", n);
    //      if (op->mem.base != ARM64_REG_INVALID)
    //        printf("\t\t\toperands[%u].mem.base: REG = %s\n", n, cs_reg_name(handle, op->mem.base));
    //      if (op->mem.index != ARM64_REG_INVALID)
    //        printf("\t\t\toperands[%u].mem.index: REG = %s\n", n, cs_reg_name(handle, op->mem.index));
    //      if (op->mem.disp != 0)
    //        printf("\t\t\toperands[%u].mem.disp: 0x%x\n", n, op->mem.disp);
    //      continue;
    //    case ARM64_OP_CIMM:
    //       printf("\t\toperands[%u].type: C-IMM = %u\n", n, op->imm);
    //      continue;
    //    }
    //  }
     return true;
  });

}

}
