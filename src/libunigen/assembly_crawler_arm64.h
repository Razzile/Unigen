/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  assembly_crawler_arm64.h                                         *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#pragma once

#include "assembly_crawler.h"
#include <map>
#include <vector>

namespace arm64 {

class AssemblyCrawler : public base::AssemblyCrawler {
public:
    AssemblyCrawler(base::BinaryContainer *container) : base::AssemblyCrawler(container) {}

    bool Crawl(uintptr_t start, char *code, uintptr_t *meta_out, uintptr_t *code_out) override;

private:
    void MakeBLPass(Disassembler &dis, std::vector<uintptr_t> &out);
    void MakeADRPass(Disassembler &dis, std::map<uint32_t, uintptr_t> &out);
};

}
