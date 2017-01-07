// Unigen.cpp : Defines the entry point for the console application.
//

#include "il2cpp.h"
#include "FileStream.h"
#include "MetadataObject.h"
#include "getopt.h"
#include <gflags/gflags.h>

#ifdef _WIN32
#define HELP_PATH "Unigen\\src"
#else
#define HELP_PATH "Unigen/src"
#endif

DEFINE_string(metadata, "global-metadata.dat", "global-metadata file needed by Unigen");
DEFINE_string(binary, "a.out", "binary file (either mach-o or libil2cpp.so) needed by Unigen");
// below definitions are incompatible with 64 bit
DEFINE_uint32(metadata_registration, 0x0, "location of g_MetadataRegistration in binary");
DEFINE_uint32(code_registration, 0x0, "location of g_CodeRegistration in binary");

DECLARE_bool(help);
DECLARE_string(helpmatch);

int main(int argc, char **argv) {
  // customize --help output
  gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
  gflags::SetUsageMessage("unigen -metadata=\"gloabal_metadata.dat\ -binary=\"libil2cpp.so\"\
                          -metadata_registration=0x12345 -code_registration=0x12345");
  gflags::SetVersionString("0.1.0");
  if (FLAGS_help) {
    FLAGS_help = false;
    FLAGS_helpmatch = HELP_PATH;
  }
  gflags::HandleCommandLineHelpFlags();

  MemoryStream *stream = new MemoryStream(FLAGS_metadata);
  MemoryStream *binary = new MemoryStream(FLAGS_binary);
  MetadataObject obj(stream, binary, FLAGS_metadata_registration, FLAGS_code_registration);
  obj.Parse();
}
