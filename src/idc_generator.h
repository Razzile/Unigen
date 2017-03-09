#pragma once

#include "utils.h"

class MetadataParser;
namespace base {

class IDCGenerator {
public:
    IDCGenerator(MetadataParser *parser, char seperator) : parser_(parser), seperator_(seperator) {}
    virtual bool GenerateIDC(std::string out) = 0;
    // TODO: MakeFunctionString and MakeTypeString that aren't version-linked
protected:
    MetadataParser *parser_;
    char seperator_;

    std::string MakeFunctionString(uintptr_t addr, const char *cls, const char *method) {
        return string_format("MakeNameEx(0x%x, \"%s%c%c%s\", SN_NOWARN);\n", addr,
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
};

}