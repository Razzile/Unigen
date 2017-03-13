/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  metadata_loader.h                                                *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once

#include "memory_stream.h"

namespace base {

class MetadataLoader {
public:
  MetadataLoader(MemoryStream *metadata, MemoryStream *binary)
    : metadata_(metadata), binary_(binary) {}

  bool GenerateIDC(std::string outPath);
  bool GenerateHeaders(std::string outPath);
private:
  uint32_t GetMetadataVersion();
  MemoryStream *metadata_, *binary_;
};

}
