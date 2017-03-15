/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  metadata_parser.cc                                               *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "metadata_parser.h"

namespace versions {
namespace v21 {

void MetadataParser::Load() {
  auto &stream = metadata_;
  auto &header = header_;
  header = stream->Read<Il2CppGlobalMetadataHeader>(0x0);
  loaded_ = true;
}

std::vector<Il2CppImage> MetadataParser::Images() {
  if (loaded_) {
    auto &header = header_;
    auto &stream = metadata_;

    std::vector<Il2CppImage> images;

    int image_data_size = header.imagesCount;
    int image_count = (image_data_size / sizeof(Il2CppImageDefinition));
    Il2CppImageDefinition *image_data =
        stream->View<Il2CppImageDefinition>(header.imagesOffset);
    for (int i = 0; i < image_count; i++) {
      Il2CppImageDefinition *def = &image_data[i];
      Il2CppImage image;
      image.name = StringLookup(def->nameIndex);
      image.assemblyIndex = def->assemblyIndex;
      image.typeStart = def->typeStart;
      image.typeCount = def->typeCount;
      image.entryPointIndex = def->entryPointIndex;
      image.token = def->token;

      images.push_back(image);
    }
    return images;
  } else {
    // pro skills coding
    Load();
    return Images();
  }
}

std::vector<Il2CppClass> MetadataParser::ClassesFromImage(Il2CppImage &img) {
  std::vector<Il2CppClass> classes;
  for (int i = 0; i < img.typeCount; i++) {
    Il2CppClass cls = ClassFromTypeDefinition(img.typeStart + i);
    classes.push_back(cls);
  }
  return classes;
}

std::vector<MethodInfo> MetadataParser::MethodsFromClass(Il2CppClass &cls) {
  std::vector<MethodInfo> methods;
  int start = cls.typeDefinition->methodStart;
  int end = start + cls.method_count;

  for (int i = start; i < end; i++) {
    MethodInfo info = MethodFromMethodDefinition(i);
    info.declaring_type = &cls;
    methods.push_back(info);
  }
  return methods;
}

Il2CppClass MetadataParser::ClassFromTypeDefinition(TypeDefinitionIndex index) {
  Il2CppClass temp;

  size_t td_offset =
      header_.typeDefinitionsOffset + (index * sizeof(Il2CppTypeDefinition));

  Il2CppTypeDefinition *definition =
      metadata_->View<Il2CppTypeDefinition>(td_offset);

  temp.name = StringLookup(definition->nameIndex);
  temp.namespaze = StringLookup(definition->namespaceIndex);
  temp.customAttributeIndex = definition->customAttributeIndex;
  temp.typeDefinition = definition;
  temp.genericContainerIndex = definition->genericContainerIndex;
  temp.thread_static_fields_offset = -1;
  temp.flags = definition->flags;
  temp.is_generic =
      (definition->genericContainerIndex != -1); // -1 may be wrong
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

MethodInfo MetadataParser::MethodFromMethodDefinition(MethodIndex index) {
  MethodInfo temp;

  // will leak
  Il2CppMethodDefinition *definition = metadata_->View<Il2CppMethodDefinition>(
      header_.methodsOffset + (index * sizeof(Il2CppMethodDefinition)));

  // TODO: copy more properties
  temp.name = StringLookup(definition->nameIndex);
  temp.is_generic = (definition->genericContainerIndex != -1);
  temp.is_inflated = false;
  temp.flags = definition->flags;
  temp.iflags = definition->iflags;
  temp.slot = definition->slot;
  temp.parameters_count = definition->parameterCount;
  temp.token = definition->token;
  temp.methodDefinition = definition;
  temp.method = MethodPointerFromIndex(definition->methodIndex);
  temp.return_type = TypeFromTypeIndex(definition->returnType);

  ParameterInfo *parameters =
      new ParameterInfo[definition->parameterCount];
  for (int i = 0; i < definition->parameterCount; i++) {
    uint32_t offset =
        header_.parametersOffset + (i * sizeof(Il2CppParameterDefinition));
    auto *param_def = metadata_->View<Il2CppParameterDefinition>(offset);
    ParameterInfo &info = parameters[i];

    info.name = StringLookup(param_def->nameIndex);
    info.token = param_def->token;
    info.parameter_type = TypeFromTypeIndex(param_def->typeIndex);
  }
  temp.parameters = parameters;

  return temp;
}

const char *MetadataParser::StringLookup(StringIndex nameIndex) {
  metadata_->set_offset(header_.stringOffset + nameIndex);
  std::vector<char> chars;

  while (true) {
    char c = metadata_->ReadChar();
    chars.push_back(c);
    if (c == '\0')
      break;
  }
  char *string = new char[chars.size()];
  std::memcpy(string, &chars[0], chars.size());
  return string;
}

Il2CppType *MetadataParser::TypeFromTypeIndex(TypeIndex index) {
  auto *metadata_reg = binary_->View<Il2CppMetadataRegistration>(metadata_registration_);
  Il2CppType **types = (Il2CppType **)binary_->MapPtr((void *)((uintptr_t)metadata_reg->types /*- 0x100000000*/));
  Il2CppType *type = (Il2CppType *)binary_->MapPtr(types[index]);

  return type;
}

Il2CppMethodPointer MetadataParser::MethodPointerFromIndex(MethodIndex index) {
  auto *code_reg = binary_->View<Il2CppCodeRegistration>(code_registration_);
  Il2CppMethodPointer *types = (Il2CppMethodPointer *)binary_->MapPtr((void *)((uintptr_t)(code_reg->methodPointers) /*- 0x100000000*/));

  Il2CppMethodPointer pointer = (Il2CppMethodPointer)types[index];
  return pointer;
}

}
}
