#pragma once

#include "MetadataObject.h"

class IDCGenerator {
public:
  IDCGenerator(MetadataObject *metadata, std::string output)
      : metadata_(metadata), output_(output), seperator_('$') {}
  IDCGenerator(MetadataObject *metadata, std::string output, char seperator)
      : metadata_(metadata), output_(output), seperator_(seperator) {}

  void Run(bool typed);

private:
  std::string MakeFunctionString(uint32_t addr, const char *cls,
                                 const char *method);
  std::string MakeTypeString(uint32_t addr, const Il2CppType *rtn,
                             const char *cls, const char *method,
                             const ParameterInfo *params, int param_count);

  const char *TypeNameFromType(const Il2CppType *type);
  MetadataObject *metadata_;
  std::string output_;
  char seperator_;
};
