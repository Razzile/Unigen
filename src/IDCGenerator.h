#pragma once

#include "MetadataObject.h"

class IDCGenerator {
public:
  IDCGenerator(MetadataObject *metadata)
      : metadata_(metadata), seperator_('$') {}
  IDCGenerator(MetadataObject *metadata, char seperator)
      : metadata_(metadata), seperator_(seperator) {}

  std::string Run();

private:
  const char *TypeNameFromType(const Il2CppType *type);
  MetadataObject *metadata_;
  char seperator_;
};
