/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  assembly_crawler.h                                               *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#pragma once

#include "disassembler.h"
#include "binary.h"

namespace base {

class AssemblyCrawler {
public:
  AssemblyCrawler(Binary *binary) : binary_(binary) {}
  
  virtual bool Crawl(uintptr_t start, char *code, uintptr_t *meta_out, uintptr_t *code_out) {
    throw std::runtime_error("something has gone terribly wrong");
  }

protected:
  Binary *binary_;
};

}
