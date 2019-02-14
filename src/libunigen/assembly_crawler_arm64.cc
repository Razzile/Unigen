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

using base::SectionType;

bool AssemblyCrawler::Crawl(uintptr_t start, char *code, uintptr_t *meta_reg_out, uintptr_t *code_reg_out) {
    Disassembler dis(start, (const uint8_t *) code, 32, Arch::ARM64, Mode::ModeARM);
    dis.SetupDisassembler();
    uintptr_t target_func = 0x0; // the target func that contains the structs
    const uint8_t *target_code = nullptr;

    std::vector<uintptr_t> bl_addresses;
    MakeBLPass(dis, bl_addresses);

    if (bl_addresses.size() > 0) {
        for (uintptr_t &addr : bl_addresses) {
            const uint8_t *target_code = (const uint8_t *) (code - start) + addr;
            dis.set_address(addr);
            dis.set_code(target_code);

            std::map<uint32_t, uintptr_t> adrp_refs;
            MakeADRPass(dis, adrp_refs);
            auto pair = adrp_refs.find(ARM64_REG_X1);
            if (pair != adrp_refs.end()) {
                // we found an ADRP that is stored in X1
                // check if pair->second is in __TEXT or __DATA
                SectionType type = container_->SectionTypeForAddress(pair->second);
                if (type == SectionType::TEXT) {
                    printf("TEXT: 0x%llx\n", pair->second);
                } else if (type == SectionType::DATA) {
                    printf("DATA: 0x%llx\n", pair->second);
                } else {
                    return false;
                }
            }
        }
    }
    dis.set_address(start);
    dis.set_code((const uint8_t *) code);
    std::map<uint32_t, uintptr_t> adrp_refs;
    MakeADRPass(dis, adrp_refs);
    auto pair = adrp_refs.find(ARM64_REG_X1);
    if (pair != adrp_refs.end()) {
        // we found an ADRP that is stored in X1
        // check if pair->second is in __TEXT or __DATA
        SectionType type = container_->SectionTypeForAddress(pair->second);
        if (type == SectionType::TEXT) {
            printf("TEXT: 0x%llx\n", pair->second);
        } else if (type == SectionType::DATA) {
            printf("DATA: 0x%llx\n", pair->second);
        } else {
            return false;
        }
    }
    return false;
}

void AssemblyCrawler::MakeBLPass(Disassembler &dis, std::vector<uintptr_t> &out) {
    const int max_count = 8; // stop 8 instructions deep
    int count = 0;
    dis.DisassembleIter([&](Instr instr) -> bool {
        uint32_t id = instr.id();
        cs_detail *detail = instr.detail();
        csh handle = dis.handle();

        switch (id) {
            case ARM64_INS_BL: {
                cs_arm64_op op = detail->arm64.operands[0];
                out.push_back((uintptr_t) op.imm);

                break;
            }
            case ARM64_INS_RET: {
                // TODO: maybe check for other function epilogues
                return false; // reached end of function
            }
        }
        if (count++ >= max_count) {
            return false;
        } else {
            return true;
        }
    });
}

void AssemblyCrawler::MakeADRPass(Disassembler &dis, std::map<uint32_t, uintptr_t> &out) {
    const int max_count = 16; // stop 16 instructions deep
    int count = 0;
    dis.DisassembleIter([&](Instr instr) -> bool {
        uint32_t id = instr.id();
        cs_detail *detail = instr.detail();
        csh handle = dis.handle();

        switch (id) {
            case ARM64_INS_ADRP: {
                uint64_t adrp_access_off = 0x0;
                uint32_t reg_write = 0x0;
                int i;
                for (i = 0; i < detail->arm64.op_count; i++) {
                    cs_arm64_op *op = &(detail->arm64.operands[i]);
                    if (op->type == ARM64_OP_IMM) {
                        adrp_access_off = op->imm;
                        break;
                    }
                }

                Instr next = dis.DisassembleNext(instr);

                uint32_t id = next.id();
                cs_detail *detail = next.detail();
                //if (next.ReadsReg(handle, detail->))
                for (i = 0; i < detail->arm64.op_count; i++) {
                    cs_arm64_op *op = &(detail->arm64.operands[i]);
                    if (op->type == ARM64_OP_IMM) {
                        adrp_access_off += op->imm;
                    } else if (op->type == ARM64_OP_REG) {
                        if (next.WritesReg(handle, op->reg)) {
                            reg_write = op->reg;
                        }
                    }
                }

                if (adrp_access_off && reg_write) {
                    // insert address into map with the register addr is stored in as key
                    out.emplace(reg_write, adrp_access_off);
                }

                break;
            }

            case ARM64_INS_ADR: {
                uint64_t adr_access_off = 0x0;
                uint32_t reg_write = 0x0;
                int i;
                for (i = 0; i < detail->arm64.op_count; i++) {
                    cs_arm64_op *op = &(detail->arm64.operands[i]);
                    if (op->type == ARM64_OP_IMM) {
                        adr_access_off = op->imm;
                    } else if (op->type == ARM64_OP_REG) {
                        if (instr.WritesReg(handle, op->reg)) {
                            reg_write = op->reg;
                        }
                    }
                }

                if (adr_access_off && reg_write) {
                    // insert address into map with the register addr is stored in as key
                    out.emplace(reg_write, adr_access_off);
                }

                break;
            }

            case ARM64_INS_RET: {
                // TODO: maybe check for other function epilogues
                return false; // reached end of function
            }
        }
        if (count++ >= max_count) {
            return false;
        } else {
            return true;
        }
    });
}
}

