#include "Binary.h"

static inline Il2CppMetadataUsage GetEncodedIndexType(EncodedMethodIndex index) {
  return (Il2CppMetadataUsage)((index & 0xE0000000) >> 29);
}

static inline uint32_t GetDecodedMethodIndex(EncodedMethodIndex index) {
  return index & 0x1FFFFFFFU;
}

bool Binary::Parse() {
  return true; // this function may be un-needed
}

uintptr_t Binary::FindMetadataRegistration() {
  if (metadata_registration_) return metadata_registration_;
  return 0; // TODO: auto finding
}

uintptr_t Binary::FindCodeRegistration() {
  if (code_registration_) return code_registration_;
  return 0;
}

Il2CppType *Binary::TypeFromTypeIndex(TypeIndex index) {
  auto *metadata_reg = stream_->View<Il2CppMetadataRegistration>(metadata_registration_);
  Il2CppType **types = (Il2CppType **)stream_->MapPtr(metadata_reg->types);
  Il2CppType *type = (Il2CppType *)stream_->MapPtr(types[index]);

  return type;
}

Il2CppMethodPointer Binary::MethodPointerFromIndex(MethodIndex index) {
  auto *code_reg = stream_->View<Il2CppCodeRegistration>(code_registration_);
  Il2CppMethodPointer *types = (Il2CppMethodPointer *)stream_->MapPtr(code_reg->methodPointers);

  Il2CppMethodPointer pointer = (Il2CppMethodPointer)types[index];
  return pointer;
}

