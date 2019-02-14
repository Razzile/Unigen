/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  metadata_parser.h                                                *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once

#include "memory_stream.h"
#include "binary_container.h"
#include "il2cpp.h"
#include <vector>

namespace versions {
namespace v21 {

using base::BinaryContainer;

class MetadataParser {
public:
  MetadataParser(MemoryStream *metadata, BinaryContainer *container, uintptr_t m_reg, uintptr_t c_reg) :
    metadata_(metadata),
    container_(container),
    loaded_(false),
    metadata_registration_(m_reg),
    code_registration_(c_reg) {}

  // TODO: migrate code from MetadataObject.cpp
  void Load();

  std::vector<Il2CppImage> Images();
  std::vector<Il2CppClass> ClassesFromImage(Il2CppImage &image);
  std::vector<MethodInfo> MethodsFromClass(Il2CppClass &cls);
private:
  Il2CppImage ImageFromImageDefinition(ImageIndex index);
  Il2CppClass ClassFromTypeDefinition(TypeDefinitionIndex index);
  MethodInfo MethodFromMethodDefinition(MethodIndex index);
  Il2CppType *TypeFromTypeIndex(TypeIndex index);
  Il2CppMethodPointer MethodPointerFromIndex(MethodIndex index);

const char *TypeNameFromType(const Il2CppType *type);
  const char *StringLookup(StringIndex index);

  MemoryStream *metadata_;
  BinaryContainer *container_;
  bool loaded_;
  Il2CppGlobalMetadataHeader header_;
  uintptr_t metadata_registration_; // g_MetadataRegistration location
  uintptr_t code_registration_; // g_CodeRegistration location
};

}
}
