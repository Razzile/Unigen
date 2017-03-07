#pragma once

#include <string>

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
};

}