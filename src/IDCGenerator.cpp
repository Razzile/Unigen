#include "IDCGenerator.h"
#include <fstream>
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

void IDCGenerator::Run(bool typed) {
  auto binary = metadata_->binary();
  std::string output = "#include <idc.idc>\nstatic main() {\n";

  metadata_->Parse();
  binary->Parse();

  // loop over every image, then loop over every class in image
  // finally loop over every method in class, creating an idc stub for each
  // TODO: this code is messy
  std::vector<Il2CppImage> images = metadata_->images();
  for (Il2CppImage &image : images) {
    for (Il2CppClass &cls : metadata_->ClassesFromImage(image)) {
      for (MethodInfo &info : metadata_->MethodsFromClass(cls)) {
        uintptr_t pointer = (uintptr_t)info.method & ~1;
        output += MakeFunctionString(pointer, cls.name, info.name);

        // output typ info for IDC too (needs major work)
        if (typed) {
          output +=
              MakeTypeString(pointer, info.return_type, cls.name, info.name,
                             info.parameters, info.parameters_count);
        }
      }
    }
  }
  output += "}";
  std::ofstream stream(output_);
  stream << output;
  stream.close();
}

std::string IDCGenerator::MakeFunctionString(uint32_t addr, const char *cls,
                                             const char *method) {
  return string_format("MakeNameEx(0x%x, \"%s%c%c%s\", SN_NOWARN);\n", addr,
                       cls, seperator_, seperator_, method);
}

std::string IDCGenerator::MakeTypeString(uint32_t addr, const Il2CppType *rtn,
                                         const char *cls, const char *method,
                                         const ParameterInfo *params,
                                         int param_count) {
  std::string full =
      string_format("SetType(0x%x, \"%s %s%c%c%s(", addr, TypeNameFromType(rtn),
                    cls, seperator_, seperator_, method);
  for (int i = 0; i < param_count; i++) {
    const ParameterInfo *param = &params[i];
    full += string_format("%s %s", TypeNameFromType(param->parameter_type),
                          param->name);
    if (i + 1 != param_count)
      full += ", ";
  }
  full += ");\");\n";
  return full;
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
