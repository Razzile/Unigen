/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  metadata_loader.cc                                               *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "metadata_loader.h"
#include <gflags/gflags.h>

#include "versions/21/version.h"

#if _WIN64 || __x86_64__
DECLARE_uint64(metadata_registration);
DECLARE_uint64(code_registration);
#else
DECLARE_uint32(metadata_registration);
DECLARE_uint32(code_registration);
#endif

namespace base {

bool MetadataLoader::GenerateIDC(std::string path) {
  uint32_t version = GetMetadataVersion();
  printf("%d\n", version);
  switch (version) {
    case 21:
    case 22: {
      namespace target = versions::v21;
      auto parser = target::MetadataParser(metadata_, binary_,
        FLAGS_metadata_registration, FLAGS_code_registration);

      auto idc_generator = target::IDCGenerator(&parser);
      return idc_generator.GenerateIDC(path);
      break;
    }

    default: {
      return false;
    }
  }
}

bool MetadataLoader::GenerateHeaders(std::string path) {
  // TODO:
  return false;
}

bool MetadataLoader::IsMetadataValid() {
  if (metadata_) {
    metadata_->set_offset(0x0);
    uint32_t sanity = metadata_->ReadUInt();
    return (sanity == 0xFAB11BAF);
  }
  return false;
}

uint32_t MetadataLoader::GetMetadataVersion() {
  if (metadata_) {
    if (IsMetadataValid()) {
      // metadata version located at 0x4 in global-metadata.dat
      metadata_->set_offset(0x4);
      return metadata_->ReadUInt();
    }
  }
  return -1;
}

}
