#pragma once

#include "MemoryStream.h"
#include "il2cpp.h"

class Binary {
friend class IDCGenerator;
public:
  Binary(MemoryStream *stream) : stream_(stream), 
    metadata_registration_(0x0), code_registration_(0x0) {}
  Binary(MemoryStream *stream, uintptr_t mReg, uintptr_t cReg) : stream_(stream),
    metadata_registration_(mReg), code_registration_(cReg) {}

  bool Parse();

  uintptr_t FindMetadataRegistration();
  uintptr_t FindCodeRegistration();

protected:
  Il2CppType *TypeFromTypeIndex(TypeIndex index);
  Il2CppMethodPointer MethodPointerFromIndex(MethodIndex index);

  const char *TypeNameFromType(const Il2CppType *type); // relocate?

private:
  MemoryStream *stream_;
  uintptr_t metadata_registration_; // g_MetadataRegistration location
  uintptr_t code_registration_; // g_CodeRegistration location
};