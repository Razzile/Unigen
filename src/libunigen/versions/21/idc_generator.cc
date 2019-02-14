/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  idc_generator.cc                                                 *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#include "idc_generator.h"
#include <fstream>

bool versions::v21::IDCGenerator::GenerateIDC(std::string out) {
 parser_->Load();
 auto images = parser_->Images();
 std::string output = "#include <idc.idc>\nstatic main() {\n";
 for (Il2CppImage image : images) {
   for (Il2CppClass &cls : parser_->ClassesFromImage(image)) {
     for (MethodInfo &info : parser_->MethodsFromClass(cls)) {
       uintptr_t pointer = (uintptr_t)info.method & ~1;
       output += MakeFunctionString(pointer, cls.name, info.name);

       // output typ info for IDC too (needs major work)
      //  if (0) {
      //    output +=
      //        MakeTypeString(pointer, info.return_type, cls.name, info.name,
      //                       info.parameters, info.parameters_count);
      //  }
     }
   }
 }
 output += "}";
 std::ofstream stream(out);
 stream << output;
 stream.close();

 return true;
}
