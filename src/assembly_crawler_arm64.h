/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  assembly_crawler_arm64.h                                         *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "assembly_crawler.h"

namespace arm64 {

class AssemblyCrawler : public base::AssemblyCrawler {
public:
  AssemblyCrawler() : base::AssemblyCrawler() {}
  bool Crawl(uintptr_t start, char *code, uintptr_t *meta_out, uintptr_t *code_out) override;

private:
};

}
