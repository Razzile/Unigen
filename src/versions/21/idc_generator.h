/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  idc_generator.h                                                  *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */
#pragma once

#include "../../idc_generator.h"
#include "metadata_parser.h"

namespace versions {
namespace v21 {

class IDCGenerator : base::IDCGenerator {
public:
    IDCGenerator(MetadataParser *parser)
      : base::IDCGenerator(), parser_(parser) {}
    IDCGenerator(MetadataParser *parser, char seperator)
      : base::IDCGenerator(seperator), parser_(parser) {}  
    bool GenerateIDC(std::string out) override;

  private:
    MetadataParser *parser_;
};

}
}
