#pragma once

#include "MemoryStream.h"
#include "il2cpp.h"
#include <vector>

static const char *type_names[] = {
  "END",
  "void",
  "bool",
  "char",
  "sbyte",// I1
  "byte",// U1
  "short",// I2
  "ushort",// U2
  "int",// I4
  "uint",// U4
  "long",// I8
  "ulong",// U8
  "float",// R4
  "double",// R8
  "String",
  "Ptr",
  "REF",
  "ValueType",
  "Class",
  "var",
  "array",
  "Generic",
  "TypedREF",
  "int",// I
  "uint",// U
  "FuncPtr",
  "Object",
  "sArray",
  "mvar",
  "cmod_reqd",
  "cmod_opt",
  "internal_type",
  "modifier",
  "sentinel",
  "pinned",
  "enum"
};

class MetadataObject {
public:
  MetadataObject(MemoryStream *metadata, MemoryStream *binary,
    uintptr_t mReg, uintptr_t cReg) : stream_(metadata), binary_(binary),
    mReg_(mReg), cReg_(cReg)  {};
  bool Parse();

  inline const Il2CppGlobalMetadataHeader& header() const { return *header_; }
  inline std::vector<Il2CppImage>& images() const { return images_; }
private:
  inline MemoryStream* stream() const { return stream_; }

  bool ParseImages();
  bool ParseClasses();

  std::vector<Il2CppClass> ClassesFromImage(Il2CppImage img);
  std::vector<MethodInfo> MethodsFromClass(Il2CppClass cls);

  Il2CppClass ClassFromTypeDefinition(TypeDefinitionIndex index);
  MethodInfo MethodFromMethodDefinition(MethodIndex index);

  // TODO: relocate these
  Il2CppType *TypeFromTypeIndex(TypeIndex index); 
  methodPointerType *MethodPointerFromIndex(MethodIndex index);

  Il2CppGenericContainer *GenericContainerFromIndex(GenericContainerIndex index); // metadata header

  const char *StringLookup(StringIndex nameIndex);
  const char *TypeNameFromType(const Il2CppType *type); // relocate?

  MemoryStream *stream_;
  MemoryStream *binary_;
  uintptr_t mReg_; // g_MetadataRegistration location
  uintptr_t cReg_; // g_CodeRegistration location
  Il2CppGlobalMetadataHeader header_;
  // may wrap these in a class
  std::vector<Il2CppImage> images_;
  std::vector<Il2CppClass> classes_;
};
