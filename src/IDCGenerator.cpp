#include "IDCGenerator.h"
#include <memory>
#include <stdarg.h>
#include <stdlib.h>

static const char *idc_type_names[] = {
    "END",      "void __cdecl", "bool",   "char",
    "int8_t",   // I1
    "uint8_t",  // U1
    "int16_t",  // I2
    "uint16_t", // U2
    "int",      // I4
    "uint",     // U4
    "long",     // I8
    "ulong",    // U8
    "float",    // R4
    "double",   // R8
    "_DWORD",   "_DWORD",       "_DWORD", "_DWORD", "_DWORD", "_DWORD",
    "char[]",   "_DWORD",       "_DWORD",
    "int",  // I
    "uint", // U
    "_DWORD",   "_DWORD",       "_DWORD", "_DWORD", "_DWORD", "_DWORD",
    "_DWORD",   "_DWORD",       "_DWORD", "_DWORD", "enum"};

std::string string_format(const std::string fmt_str, ...) {
  int final_n,
      n = ((int)fmt_str.size()) *
          2; /* Reserve two times as much as the length of the fmt_str */
  std::string str;
  std::unique_ptr<char[]> formatted;
  va_list ap;
  while (1) {
    formatted.reset(
        new char[n]); /* Wrap the plain char array into the unique_ptr */
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
  auto binary = metadata_->binary();
  std::string output = "#include <idc.idc>\nstatic main() {\n";

  metadata_->Parse();
  binary->Parse();

  std::vector<Il2CppImage> images = metadata_->images();
  for (Il2CppImage &image : images) {
    for (Il2CppClass &cls : metadata_->ClassesFromImage(image)) {
      for (MethodInfo &info : metadata_->MethodsFromClass(cls)) {
        uintptr_t pointer = (uintptr_t)info.method & ~1;
        output +=
            string_format("MakeNameEx(0x%x, \"%s%c%c%s\", SN_NOWARN);\n",
                          pointer, cls.name, seperator_, seperator_, info.name);

        output += string_format("SetType(0x%x, \"%s %s%c%c%s(", pointer,
                                TypeNameFromType(info.return_type), cls.name,
                                seperator_, seperator_, info.name);
        for (int i = 0; i < info.parameters_count; i++) {
          const ParameterInfo *param = &info.parameters[i];
          output += string_format(
              "%s %s", TypeNameFromType(param->parameter_type), param->name);
          if (i + 1 != info.parameters_count)
            output += ", ";
        }
        output += ");\");\n";
      }
    }
  }
  output += "}";
  return output;
}

const char *IDCGenerator::TypeNameFromType(const Il2CppType *type) {
  switch (type->type) {
  case IL2CPP_TYPE_CLASS:
  case IL2CPP_TYPE_VALUETYPE: {
    return "_DWORD";
  }
  case IL2CPP_TYPE_END: {
    return nullptr;
  }
  default: { return idc_type_names[type->type]; }
  }
}
