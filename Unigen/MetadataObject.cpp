#include "stdafx.h"
#include "MetadataObject.h"
#include <assert.h>

bool MetadataObject::Parse() {
  header_ = stream_->View<Il2CppGlobalMetadataHeader>(0x0);
  
  assert(header_->sanity == 0xFAB11BAF && header_->version == 21);

  ParseImages();
  for (Il2CppImage *img : images_) {
    printf("image %s\n", img->name);
    for (Il2CppClass *cls : ClassesFromImage(img)) {
      printf("\tclass %s\n", cls->name);
      for (MethodInfo *method : MethodsFromClass(cls)) {
        printf("\t\t %s::%s\n", cls->name, method->name);
      }
    }
  }

  return true;
}

bool MetadataObject::ParseImages() {
  auto &header = *header_;
  auto &stream = stream_;

  int image_data_size = header.imagesCount;
  int image_count = (image_data_size / sizeof(Il2CppImageDefinition));
  Il2CppImageDefinition *image_data = stream_->View<Il2CppImageDefinition>(header.imagesOffset);
  for (int i = 0; i < image_count; i++) {
    Il2CppImageDefinition *def = &image_data[i];
    printf("%x\n", image_data->nameIndex);
    Il2CppImage *image = new Il2CppImage;
    image->name = StringLookup(def->nameIndex);
    image->assemblyIndex = def->assemblyIndex;
    image->typeStart = def->typeStart;
    image->typeCount = def->typeCount;
    image->entryPointIndex = def->entryPointIndex;
    image->token = def->token;
   
    images_.push_back(image);
  }
  return true;
}

// redundant. now part of ParseImages()
bool MetadataObject::ParseClasses() {
  for (int i = 0; i < header_->typeDefinitionsCount / sizeof(Il2CppTypeDefinition); i++) {
    Il2CppClass *class_obj = ClassFromTypeDefinition(i);
    classes_.push_back(class_obj);
  }
  return true;
}

std::vector<Il2CppClass*> MetadataObject::ClassesFromImage(Il2CppImage *img) {
  std::vector<Il2CppClass*> classes;
  for (int i = 0; i < img->typeCount; i++) {
    Il2CppClass *cls = ClassFromTypeDefinition(img->typeStart + i);
    classes.push_back(cls);
  }
  return classes;
}

Il2CppClass *MetadataObject::ClassFromTypeDefinition(TypeDefinitionIndex index) {
  Il2CppClass *temp = new Il2CppClass;

  size_t td_offset = header_->typeDefinitionsOffset + (index * sizeof(Il2CppTypeDefinition));

  Il2CppTypeDefinition *definition = stream_->View<Il2CppTypeDefinition>(td_offset);
  
  temp->name = StringLookup(definition->nameIndex);
  temp->namespaze = StringLookup(definition->namespaceIndex);
  temp->customAttributeIndex = definition->customAttributeIndex;
  temp->typeDefinition = definition;
  temp->genericContainerIndex = definition->genericContainerIndex;
  temp->thread_static_fields_offset = -1;
  temp->flags = definition->flags;
  temp->is_generic = (definition->genericContainerIndex != -1); // -1 may be wrong
  temp->method_count = definition->method_count;
  temp->property_count = definition->property_count;
  temp->field_count = definition->field_count;
  temp->event_count = definition->event_count;
  temp->nested_type_count = definition->nested_type_count;
  temp->vtable_count = definition->vtable_count;
  temp->interfaces_count = definition->interfaces_count;
  temp->interface_offsets_count = definition->interface_offsets_count;
  temp->token = definition->token;

  return temp;
}

Il2CppType *MetadataObject::TypeFromTypeIndex(TypeIndex index) {
 /* Il2CppType *type = stream_->Find<Il2CppType>(header_->attributeTypesOffset + (index * sizeof(Il2CppType)));
  *type = stream_->Read<Il2CppType>();

  return type;*/
  return nullptr;
}

std::vector<MethodInfo*> MetadataObject::MethodsFromClass(Il2CppClass *cls) {
  std::vector<MethodInfo*> methods;
  int start = cls->typeDefinition->methodStart;
  int end = start + cls->method_count;

  for (int i = start; i < end; i++) {
    MethodInfo *info = MethodFromMethodDefinition(i);
    info->declaring_type = cls;
    methods.push_back(info);
  }
  return methods;
}

MethodInfo *MetadataObject::MethodFromMethodDefinition(MethodIndex index) {
  MethodInfo *temp = new MethodInfo;

  // will leak
  Il2CppMethodDefinition *definition = stream_->View<Il2CppMethodDefinition>(header_->methodsOffset + (index * sizeof(Il2CppMethodDefinition)));

  // TODO: copy more properties
  temp->name = StringLookup(definition->nameIndex);
  temp->is_generic = (definition->genericContainerIndex != -1);
  temp->return_type = TypeFromTypeIndex(definition->returnType);

  return temp;
}

// unfortunately this will leak
const char *MetadataObject::StringLookup(StringIndex nameIndex) {
  stream_->set_offset(header_->stringOffset + nameIndex);
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

const char *MetadataObject::TypeNameFromType(const Il2CppType *type) {
  switch (type->type) {
  case IL2CPP_TYPE_CLASS:
  case IL2CPP_TYPE_VALUETYPE: {
    Il2CppClass *cls = ClassFromTypeDefinition(type->data.klassIndex);
    return cls->name;
  }
  case IL2CPP_TYPE_END: {
    return nullptr;
  }
  default: {
    return type_names[type->type];
  }
  }
}