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

namespace versions {
namespace v20 {

class IDCGenerator : base::IDCGenerator {
    IDCGenerator(MetadataParser *parser, char seperator) : base::IDCGenerator(parser, seperator) {}
    bool GenerateIDC(std::string out) override;
};

}
}
