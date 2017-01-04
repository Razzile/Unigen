// Unigen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "il2cpp.h"
#include "FileStream.h"
#include "MetadataObject.h"
#include "getopt.h"

int main(int argc, char **argv) {
  static struct option long_options[] = {
    /* These options set a flag. */
    //{ "verbose", no_argument,       &verbose_flag, 1 },
    //{ "brief",   no_argument,       &verbose_flag, 0 },
    /* These options don’t set a flag.
    We distinguish them by their indices. */
    { "metadata",required_argument, 0, 'm' },
    { "binary",  required_argument, 0, 'b' },
    { "mReg",    optional_argument, 0, 'r' },
    { "cReg",    optional_argument, 0, 'c' },
    { "output",  required_argument, 0, 'o' },
    { 0, 0, 0, 0 }
  };

  char *metadata_file = 0;
  char *binary_file = 0;
  uintptr_t metadata_reg_off = 0;
  uintptr_t code_reg_off = 0;

  int opt;
  while (1) {
    int index = 0;
    opt = getopt_long(argc, argv, "m:b:r:c:o:", long_options, &index);
    if (opt == -1) break;
    switch (opt) {
      case 'm': {
        metadata_file = optarg;
        break;
      }
      case 'b': {
        binary_file = optarg;
        break;
      }
      case 'r': {
        metadata_reg_off = strtoull(optarg, NULL, 0);
        break;
      }
      case 'c': {
        code_reg_off = strtoull(optarg, NULL, 0);
        break;
      }
      case 'o': {
        break;
      }
    }
  }
  MemoryStream *stream = new MemoryStream(metadata_file);
  MemoryStream *binary = new MemoryStream(binary_file);
  MetadataObject obj(stream, binary, metadata_reg_off, code_reg_off);
  obj.Parse();
}

