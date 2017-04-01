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
#include "binary.h"

namespace base {

class MetadataLoader {
public:
  MetadataLoader(MemoryStream *metadata, MemoryStream *bin_stream);
  ~MetadataLoader();

  bool GenerateIDC(std::string outPath);
  bool GenerateHeaders(std::string outPath);
private:
  bool IsMetadataValid();
  uint32_t GetMetadataVersion();

  MemoryStream *metadata_;
  Binary *binary_;
};

}
