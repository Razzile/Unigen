/**
******************************************************************************
* Unigen :  Unity metadata parser for il2cpp games                           *
* File   :  binary.cc                                                        *
******************************************************************************
* Copyright 2017 Satori. All rights reserved.                                *
******************************************************************************
*/

#include "binary.h"

namespace base {

uintptr_t Binary::ConvertVirtualAddress(uintptr_t addr) {
    // MachO:
    // find out which section addr falls under
    // return addr - that secdion's vmaddr;
    //
    // ELF:
    // TODO:
    return addr;
}

uintptr_t Binary::FindMetadataRegistration() {
    // MachO:
    // il2cpp init function is always 2nd ctor in __mod_init_func
    // use capstone to parse and check for references to other functions
    // should only be 1 function reference. 
    // go to that function and check for PC-rel LDRs into R0/X0 and R1/X1
    // R0/X0 is g_codeRegistration and R1/X1 is g_metadataRegistration
}

}