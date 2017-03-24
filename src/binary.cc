/**
******************************************************************************
* Unigen :  Unity metadata parser for il2cpp games                           *
* File   :  binary.cc                                                        *
******************************************************************************
* Copyright 2017 Satori. All rights reserved.                                *
******************************************************************************
*/

#include "binary.h"

#define PE_MAGIC 0x4D5A

#define ELF_MAGIC 0x7F454C46

#define MH_MAGIC 0xFEEDFACE
#define MH_CIGAM 0xCEFAEDFE

#define MH_MAGIC_64 0xFEEDFACF
#define MH_CIGAM_64 0xCFFAEDFE

#define FAT_MAGIC 0xCAFEBABE
#define FAT_CIGAM 0xBEBAFECA

namespace base {

BinaryType Binary::Type(MemoryStream *stream) {
  stream->set_offset(0x0);
  uint32_t magic = stream->ReadUInt();

  if (magic == MH_MAGIC ||
      magic == MH_CIGAM ||
      magic == MH_MAGIC_64 ||
      magic == MH_CIGAM_64) {
    return BinaryType::MachO;
  }
  else if (magic == FAT_MAGIC ||
           magic == FAT_CIGAM) {
    return BinaryType::FatMachO;
  }
  else if (magic == ELF_MAGIC) {
    return BinaryType::ELF;
  }
  else if (((magic >> 16) & 0xFFFF) == PE_MAGIC) {
    return BinaryType::PE;
  }
  return BinaryType::UNKNOWN;
}

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
