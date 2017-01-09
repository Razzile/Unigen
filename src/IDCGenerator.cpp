#include "IDCGenerator.h"
#include <memory>
#include <stdarg.h>

static std::string string_format(const std::string fmt_str, ...) {
  int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
  std::string str;
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
    strcpy(&formatted[0], fmt_str.c_str());
    va_start(ap, fmt_str);
    final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
    va_end(ap);
    if (final_n < 0 || final_n >= n)
      n += abs(final_n - n + 1);
    else
      break;
  }
  return std::string(formatted.get());
}

std::string IDCGenerator::Run() {
  std::string output = "#include <idc.idc>\nstatic main() {\n";

  metadata_->Parse();
  binary_->Parse();

  std::vector<Il2CppImage> images = metadata_->images();
  for (Il2CppImage &image : images) {
    for (Il2CppClass &cls : metadata_->ClassesFromImage(image)) {
      for (MethodInfo &info : metadata_->MethodsFromClass(cls)) {
        FixupMethod(info);
        uintptr_t pointer = (uintptr_t)info.method & ~1;
        output += string_format("MakeNameEx(0x%x, \"%s %s%c%c%s\", SN_NOWARN);\n",
          pointer, metadata_->TypeNameFromType(info.return_type), cls.name, 
          seperator_, seperator_, info.name);
      }
    }
  }
  output += "}";
  return output;
}

// points methodPointer and other items to the binary
void IDCGenerator::FixupMethod(MethodInfo &info) {
  const Il2CppMethodDefinition *definition = info.methodDefinition;
  info.method = binary_->MethodPointerFromIndex(definition->methodIndex);
  info.return_type = binary_->TypeFromTypeIndex(definition->returnType);
}