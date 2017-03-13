/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  metadata_loader.cc                                               *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "metadata_loader.h"

namespace base {

bool MetadataLoader::GenerateIDC(std::string path) {
  uint32_t version = GetMetadataVersion();
  switch (version) {
    case 20: {
      //versions::v20::
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

uint32_t MetadataLoader::GetMetadataVersion() {
  if (metadata_) {
    // TODO: verify metadata is valid
    // metadata version located at 0x4 in global-metadata.dat
    metadata_->set_offset(0x4);
    return metadata_->Stream::ReadUInt();
  }
  return -1;
}

}
