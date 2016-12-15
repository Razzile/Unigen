#pragma once

#include "Stream.h"
#include "il2cpp.h"
#include <vector>

class MetadataObject {
public:
  MetadataObject(Stream *stream) : stream_(stream) {};
  bool Parse();

  inline const Il2CppGlobalMetadataHeader& header() const { return header_; }
  inline std::vector<Il2CppImageDefinition> image_definitions() const { return image_definitions_;  }
  inline std::vector<Il2CppImage> images() const { return images_; }
private:
  template <typename T> T ItemLookup(size_t section, size_t item);
  const char *StringLookup(size_t nameIndex);

  Stream *stream_;
  Il2CppGlobalMetadataHeader header_;
  // may wrap these in a class
  std::vector<Il2CppImageDefinition> image_definitions_;
  std::vector<Il2CppImage> images_;
};


template <typename T>
T MetadataObject::ItemLookup(size_t section, size_t item) {
  stream_->set_offset(section + item);
  return stream_->Read<T>();
}
