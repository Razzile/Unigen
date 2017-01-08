#include "MetadataObject.h"
#include "Endian.h"
#include <assert.h>
#include <algorithm>

static bool methodinfo_sort(const MethodInfo* one, const MethodInfo* two) {
  return one->declaring_type->typeDefinition->methodStart < two->declaring_type->typeDefinition->methodStart;
}

static inline Il2CppMetadataUsage GetEncodedIndexType(EncodedMethodIndex index) {
  return (Il2CppMetadataUsage)((index & 0xE0000000) >> 29);
}

static inline uint32_t GetDecodedMethodIndex(EncodedMethodIndex index) {
  return index & 0x1FFFFFFFU;
}

bool MetadataObject::Parse() {
  auto &stream = stream_;
  header_ = stream->Read<Il2CppGlobalMetadataHeader>();

  assert(header_->sanity == 0xFAB11BAF && header_->version == 21);

  ParseImages();


  for (auto &image : images_) {
    for (auto &cls : ClassesFromImage(image)) {
      for (auto &method : MethodsFromClass(cls)) {
        const Il2CppType *type = method.return_type;
        printf("%s %s::%s\n", TypeNameFromType(type), cls->name, method->name);
      }
    }
  }

  //std::sort(methods.begin(), methods.end(), methodinfo_sort);
  //for (MethodInfo *info : methods) {
  //    //printf("%s::%s\n", info->declaring_type->name, info->name);
  //}

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

// for string literals
//std::vector<MethodInfo *> MetadataObject::OrderedMethodList() {
//  auto &stream = stream_;
//  auto *usage_list = stream->View<Il2CppMetadataUsageList>(header_->metadataUsageListsOffset);
//
//  uint32_t start = usage_list->start;
//  uint32_t count = usage_list->count;
//
//  for (int i = 0; i < count; i++) {
//    auto *pair = stream->View<Il2CppMetadataUsagePair>(header_->metadataUsagePairsOffset + start + i);
//    Il2CppMetadataUsage usage = GetEncodedIndexType(pair->encodedSourceIndex);
//    uint32_t decoded_index = GetDecodedMethodIndex(pair->encodedSourceIndex);
//
//    switch (usage) {
//    default: continue;
//      case k
//    }
//  }
//
//  return std::vector<MethodInfo *>();
//}

size_t MetadataObject::FindMetadataRegistration() {
  le<uint32_t> count = 0;
  std::vector<MethodInfo*> methods;
  for (Il2CppImage *img : images_) {
    //printf("image %s\n", img->name);
    count += img->typeCount;
    for (Il2CppClass *cls : ClassesFromImage(img)) {
      //printf("\tclass %s\n", cls->name);
      for (MethodInfo *method : MethodsFromClass(cls)) {
        methods.push_back(method);
      }
    }
  }

  uint32_t real_count = count;

  binary_->set_offset(0x0);
  while (size_t pos = binary_->Find((char *)&real_count, sizeof(real_count))) {
    if (pos == -1) break;
    if (*binary_->View<uint32_t>(pos + 8) == real_count) {
      return pos - 40;
      break;
    }
    binary_->set_offset(pos + 1);
  }
  return -1;
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
  auto *metadata_reg = binary_->View<Il2CppMetadataRegistration>(mReg_);
  Il2CppType **types = (Il2CppType **)binary_->MapPtr(metadata_reg->types);
  Il2CppType *type = (Il2CppType *)binary_->MapPtr(types[index]);

  return type;
}

methodPointerType *MetadataObject::MethodPointerFromIndex(MethodIndex index) {
  auto *code_reg = binary_->View<Il2CppCodeRegistration>(cReg_);
  methodPointerType *type = (methodPointerType *)code_reg->methodPointers[index];
  return type;
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
  temp->is_inflated = false;
  temp->return_type = TypeFromTypeIndex(definition->returnType);
  temp->flags = definition->flags;
  temp->iflags = definition->iflags;
  temp->slot = definition->slot;
  temp->parameters_count = definition->parameterCount;
  temp->token = definition->token;
  temp->methodDefinition = definition;

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
