#include "Endian.h"
#include "MetadataObject.h"
#include <algorithm>
#include <assert.h>

bool MetadataObject::Parse() {
  auto &stream = stream_;
  header_ = stream->Read<Il2CppGlobalMetadataHeader>(0x0);

  assert(header_.sanity == 0xFAB11BAF && header_.version == 21);

  ParseImages();

  return true;
}

bool MetadataObject::ParseImages() {
  auto &header = header_;
  auto &stream = stream_;

  int image_data_size = header.imagesCount;
  int image_count = (image_data_size / sizeof(Il2CppImageDefinition));
  Il2CppImageDefinition *image_data =
      stream_->View<Il2CppImageDefinition>(header.imagesOffset);
  for (int i = 0; i < image_count; i++) {
    Il2CppImageDefinition *def = &image_data[i];
    Il2CppImage image;
    image.name = StringLookup(def->nameIndex);
    image.assemblyIndex = def->assemblyIndex;
    image.typeStart = def->typeStart;
    image.typeCount = def->typeCount;
    image.entryPointIndex = def->entryPointIndex;
    image.token = def->token;

    images_.push_back(image);
  }
  return true;
}

// redundant. now part of ParseImages()
bool MetadataObject::ParseClasses() {
  for (int i = 0;
       i < header_.typeDefinitionsCount / sizeof(Il2CppTypeDefinition); i++) {
    Il2CppClass class_obj = ClassFromTypeDefinition(i);
    classes_.push_back(class_obj);
  }
  return true;
}

std::vector<Il2CppClass> MetadataObject::ClassesFromImage(Il2CppImage &img) {
  std::vector<Il2CppClass> classes;
  for (int i = 0; i < img.typeCount; i++) {
    Il2CppClass cls = ClassFromTypeDefinition(img.typeStart + i);
    classes.push_back(cls);
  }
  return classes;
}

Il2CppClass MetadataObject::ClassFromTypeDefinition(TypeDefinitionIndex index) {
  Il2CppClass temp;

  size_t td_offset =
      header_.typeDefinitionsOffset + (index * sizeof(Il2CppTypeDefinition));

  Il2CppTypeDefinition *definition =
      stream_->View<Il2CppTypeDefinition>(td_offset);

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

// Il2CppType *MetadataObject::TypeFromTypeIndex(TypeIndex index) {
//  auto *metadata_reg = binary_->View<Il2CppMetadataRegistration>(mReg_);
//  Il2CppType **types = (Il2CppType **)binary_->MapPtr(metadata_reg->types);
//  Il2CppType *type = (Il2CppType *)binary_->MapPtr(types[index]);
//
//  return type;
//}
//
// methodPointerType *MetadataObject::MethodPointerFromIndex(MethodIndex index)
// {
//  auto *code_reg = binary_->View<Il2CppCodeRegistration>(cReg_);
//  methodPointerType *type = (methodPointerType
//  *)code_reg->methodPointers[index];
//  return type;
//}

std::vector<MethodInfo> MetadataObject::MethodsFromClass(Il2CppClass &cls) {
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

MethodInfo MetadataObject::MethodFromMethodDefinition(MethodIndex index) {
  MethodInfo temp;

  // will leak
  Il2CppMethodDefinition *definition = stream_->View<Il2CppMethodDefinition>(
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
  temp.method = binary_->MethodPointerFromIndex(definition->methodIndex);
  temp.return_type = binary_->TypeFromTypeIndex(definition->returnType);

  ParameterInfo *parameters =
      new ParameterInfo[definition->parameterCount];
  for (int i = 0; i < definition->parameterCount; i++) {
    uint32_t offset =
        header_.parametersOffset + (i * sizeof(Il2CppParameterDefinition));
    auto *param_def = stream_->View<Il2CppParameterDefinition>(offset);
    ParameterInfo &info = parameters[i];

    info.name = StringLookup(param_def->nameIndex);
    info.token = param_def->token;
    info.parameter_type = binary_->TypeFromTypeIndex(param_def->typeIndex);
  }
  temp.parameters = parameters;

  return temp;
}

// unfortunately this will leak
const char *MetadataObject::StringLookup(StringIndex nameIndex) {
  stream_->set_offset(header_.stringOffset + nameIndex);
  std::vector<char> chars;

  while (true) {
    char c = stream_->ReadChar();
    chars.push_back(c);
    if (c == '\0')
      break;
  }

  char *string = new char[chars.size()];
  std::memcpy(string, &chars[0], chars.size());
  return string;
}

const char *MetadataObject::TypeNameFromType(const Il2CppType *type) {
  switch (type->type) {
  case IL2CPP_TYPE_CLASS:
  case IL2CPP_TYPE_VALUETYPE: {
    Il2CppClass cls = ClassFromTypeDefinition(type->data.klassIndex);
    return cls.name;
  }
  case IL2CPP_TYPE_END: {
    return nullptr;
  }
  default: { return type_names[type->type]; }
  }
}
