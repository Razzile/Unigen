#pragma once

#include "Stream.h"
#include "il2cpp.h"
#include <vector>

class MetadataObject {
public:
  MetadataObject(Stream *stream) : stream_(stream) {};
  bool Parse();

  inline const Il2CppGlobalMetadataHeader& header() const { return header_; }
  inline std::vector<Il2CppImage> images() const { return images_; }
private:
  bool ParseImages();
  bool ParseClasses();

  Il2CppClass ClassFromTypeDefinition(TypeDefinitionIndex index);
  template <typename T> T ItemLookup(size_t section, size_t item);
  const char *StringLookup(StringIndex nameIndex);

  Stream *stream_;
  Il2CppGlobalMetadataHeader header_;
  // may wrap these in a class
  std::vector<Il2CppImage> images_;
  std::vector<Il2CppClass> classes_;
};


template <typename T>
T MetadataObject::ItemLookup(size_t section, size_t item) {
  stream_->set_offset(section + item);
  return stream_->Read<T>();
}
