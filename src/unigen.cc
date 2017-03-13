/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  unigen.cc                                                        *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include <gflags/gflags.h>
#include <iostream>

#ifdef _WIN32
#define HELP_PATH "Unigen\\src"
#else
#define HELP_PATH "Unigen/src"
#endif

DEFINE_bool(typed, false, "output type information");

DEFINE_string(metadata, "", "global-metadata file needed by Unigen");
DEFINE_string(binary, "",
              "binary file (either mach-o or libil2cpp.so) needed by Unigen");
DEFINE_string(output, "generated.idc", "output idc file");

// below definitions are incompatible with 64 bit
DEFINE_uint32(metadata_registration, 0x0,
              "location of g_MetadataRegistration in binary");
DEFINE_uint32(code_registration, 0x0,
              "location of g_CodeRegistration in binary");

DECLARE_bool(help);
DECLARE_string(helpmatch);

int main(int argc, char **argv) {
  // customize --help output
  gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
  gflags::SetUsageMessage(
      "unigen -metadata=\"gloabal_metadata.dat\" -binary=\"libil2cpp.so\"\
                          -metadata_registration=0x12345 -code_registration=0x12345");
  gflags::SetVersionString("0.1.0");

  // setup custom help output
  if (FLAGS_help) {
    FLAGS_help = false;
    FLAGS_helpmatch = HELP_PATH;
  }
  gflags::HandleCommandLineHelpFlags();

  if (FLAGS_metadata.empty() || FLAGS_binary.empty()) {
    printf("no metadata or binary input passed.");
    return 1;
  }
}
