#include "stdafx.h"
#include "MetadataObject.h"
#include <assert.h>

bool MetadataObject::Parse() {
  stream_->set_offset(0x0);
  header_= stream_->Peek<Il2CppGlobalMetadataHeader>();
  
  assert(header_.sanity == 0xFAB11BAF && header_.version == 21);

  int image_data_size = header_.imagesCount;
  Il2CppImageDefinition *image_data = new Il2CppImageDefinition[image_data_size];
  stream_->set_offset(header_.imagesOffset);

  stream_->ReadBuf(image_data, image_data_size);
  image_definitions_.assign(image_data, image_data + image_data_size);
  delete[] image_data;

  for (const auto& def : image_definitions_) {
    Il2CppImage image;
    image.name = StringLookup(def.nameIndex);
    image.assemblyIndex = def.assemblyIndex;
    image.typeStart = def.typeStart;
    image.typeCount = def.typeCount;
    image.entryPointIndex = def.entryPointIndex;
    image.token = def.token;

    images_.push_back(image);
    printf("image %s: %u\n", image.name, image.typeStart);
  }
  return 0;
}

// unfortunately this will leak
const char *MetadataObject::StringLookup(size_t nameIndex) {
  stream_->set_offset(header_.stringOffset + nameIndex);
  std::vector<char> chars;

  while (char c = stream_->ReadChar()) chars.push_back(c);
  chars.push_back('\0');

  char *string = new char[chars.size()];
  std::memcpy(string, &chars[0], chars.size());
  return string;
}