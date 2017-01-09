#pragma once

#include "MetadataObject.h"
#include "Binary.h"

class IDCGenerator {
public:
  IDCGenerator(MetadataObject *metadata, Binary *binary) : 
    metadata_(metadata), binary_(binary), seperator_('$') {}
  IDCGenerator(MetadataObject *metadata, Binary *binary, char seperator) : 
    metadata_(metadata), binary_(binary), seperator_(seperator) {}

  std::string Run();

private:
  void FixupMethod(MethodInfo &info);
  MetadataObject *metadata_;
  Binary *binary_;
  char seperator_;
};
