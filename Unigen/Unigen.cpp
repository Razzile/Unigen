// Unigen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "il2cpp.h"
#include "FileStream.h"
#include "MetadataObject.h"

const char *Il2CppMetadataStringLookup(void *base, int index) {
  Il2CppGlobalMetadataHeader *metadata = (Il2CppGlobalMetadataHeader *)base;
  return (const char *)metadata + metadata->stringOffset + index;
}

int main(int argc, char **argv) {
  Stream *stream = new FileStream(argv[1]);
  MetadataObject obj(stream);
  obj.Parse();
  //MemoryStream *stream = new MemoryStream(argv[1]);
  //Il2CppGlobalMetadataHeader metadata = stream->Read<Il2CppGlobalMetadataHeader>();
  //assert(metadata.sanity == 0xFAB11BAF && metadata.version == 21);

  //int image_count = metadata.imagesCount / sizeof(Il2CppImageDefinition);

  //char *base = (char *)stream->base();

  //Il2CppImageDefinition *image_def_offset = (Il2CppImageDefinition *)(base + metadata.imagesOffset);
  //std::vector<Il2CppImageDefinition> image_definitions(image_count);
  //image_definitions.assign(image_def_offset, image_def_offset + image_count);

  //std::vector<Il2CppImage> images;
  //for (auto& def : image_definitions) {
  //  Il2CppImage image;
  //  image.name = Il2CppMetadataStringLookup(stream->base(), def.nameIndex);
  //  image.assemblyIndex = def.assemblyIndex;
  //  image.typeStart = def.typeStart;
  //  image.typeCount = def.typeCount;
  //  image.entryPointIndex = def.entryPointIndex;
  //  image.token = def.token;

  //  printf("image %s: %u\n", image.name, image.typeStart);
  //}
}

