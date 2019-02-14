/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  idc_generator.h                                                  *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once

#include "utils.h"

namespace base {

class IDCGenerator {
public:
    IDCGenerator(char seperator)
      : seperator_(seperator) {}
    IDCGenerator()
      : IDCGenerator('$') {}

    virtual bool GenerateIDC(std::string out) = 0;
protected:
    std::string MakeFunctionString(uintptr_t addr, const char *cls, const char *method) {
        return string_format("MakeNameEx(0x%llx, \"%s%c%c%s\", SN_NOWARN);\n", addr,
            cls, seperator_, seperator_, method);
    }

    std::string MakeTypeString(uintptr_t addr, const char *rtn, const char *cls, const char *method, const char **params, const char **types, int param_cnt) {
        std::string full = string_format("SetType(0x%x, \"%s %s%c%c%s(", addr, rtn, cls, seperator_, seperator_, method);
        for (int i = 0; i < param_cnt; i++) {
            full += string_format("%s %s", types[i], params[i]);
            if (i + 1 != param_cnt) {
                full += ", ";
            }
        }
        full += ");\");\n";
        return full;
    }
    char seperator_;
};

}
