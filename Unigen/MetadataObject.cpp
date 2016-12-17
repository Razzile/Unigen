#include "stdafx.h"
#include "MetadataObject.h"
#include <assert.h>

bool MetadataObject::Parse() {
  stream_->set_offset(0x0);
  header_= stream_->Peek<Il2CppGlobalMetadataHeader>();
  
  assert(header_.sanity == 0xFAB11BAF && header_.version == 21);

  ParseImages();
  //ParseClasses();
  return true;
}

bool MetadataObject::ParseImages() {
  int image_data_size = header_.imagesCount;
  Il2CppImageDefinition *image_data = new Il2CppImageDefinition[image_data_size];
  stream_->set_offset(header_.imagesOffset);

  stream_->ReadBuf(image_data, image_data_size);
  std::vector<Il2CppImageDefinition> image_definitions;
  image_definitions.assign(image_data, image_data + (image_data_size / sizeof(Il2CppImageDefinition)));
  delete[] image_data;

  for (const auto &def : image_definitions) {
    Il2CppImage image;
    image.name = StringLookup(def.nameIndex);
    image.assemblyIndex = def.assemblyIndex;
    image.typeStart = def.typeStart;
    image.typeCount = def.typeCount;
    image.entryPointIndex = def.entryPointIndex;
    image.token = def.token;

    printf("image %s: %u\n", image.name, image.typeStart);
    for (int i = 0; i < image.typeCount; i++) {
      Il2CppClass cls = ClassFromTypeDefinition(image.typeStart + i);
      printf("\tclass %s: methods: %d\n", cls.name, cls.method_count);
    }
   
    images_.push_back(image);
  }
  return true;
}

// redundant. now part of ParseImages()
bool MetadataObject::ParseClasses() {
  for (int i = 0; i < header_.typeDefinitionsCount / sizeof(Il2CppTypeDefinition); i++) {
    Il2CppClass class_obj = ClassFromTypeDefinition(i);
    classes_.push_back(class_obj);
  }
  return true;
}

Il2CppClass MetadataObject::ClassFromTypeDefinition(TypeDefinitionIndex index) {
  Il2CppClass temp;
  stream_->set_offset(header_.typeDefinitionsOffset + (index * sizeof(Il2CppTypeDefinition)));

  Il2CppTypeDefinition *definition = new Il2CppTypeDefinition;
  *definition = stream_->Read<Il2CppTypeDefinition>();
  
  temp.name = StringLookup(definition->nameIndex);
  temp.namespaze = StringLookup(definition->namespaceIndex);
  temp.customAttributeIndex = definition->customAttributeIndex;
  temp.typeDefinition = definition;
  temp.genericContainerIndex = definition->genericContainerIndex;
  temp.thread_static_fields_offset = -1;
  temp.flags = definition->flags;
  temp.is_generic = (definition->genericContainerIndex != -1); // -1 may be wrong
  temp.method_count = definition->method_count;
  temp.property_count = definition->property_count;
  temp.field_count = definition->field_count;
  temp.event_count = definition->event_count;
  temp.nested_type_count = definition->nested_type_count;
  temp.vtable_count = definition->vtable_count;
  temp.interfaces_count = definition->interfaces_count;
  temp.interface_offsets_count = definition->interface_offsets_count;
  temp.token = definition->token;

  return temp;
}

// unfortunately this will leak
const char *MetadataObject::StringLookup(StringIndex nameIndex) {
  stream_->set_offset(header_.stringOffset + nameIndex);
  std::vector<char> chars;

  while (true) {
    char c = stream_->ReadChar();
    chars.push_back(c);
    if (c == '\0') break;
  }

  char *string = new char[chars.size()];
  std::memcpy(string, &chars[0], chars.size());
  return string;
}