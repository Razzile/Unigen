/**
 ******************************************************************************
 * Unigen :  Unity metadata parser for il2cpp games                           *
 * File   :  macho_binary.cc                                                  *
 ******************************************************************************
 * Copyright 2017 Satori. All rights reserved.                                *
 ******************************************************************************
 */

#include "macho_binary.h"

#define MH_MAGIC 0xFEEDFACE
#define MH_CIGAM 0xCEFAEDFE

#define MH_MAGIC_64 0xFEEDFACF
#define MH_CIGAM_64 0xCFFAEDFE

#define LC_REQ_DYLD 0x80000000

/* Constants for the cmd field of all load commands, the type */
#define	LC_SEGMENT	0x1	/* segment of this file to be mapped */
#define	LC_SYMTAB	0x2	/* link-edit stab symbol table info */
#define	LC_SYMSEG	0x3	/* link-edit gdb symbol table info (obsolete) */
#define	LC_THREAD	0x4	/* thread */
#define	LC_UNIXTHREAD	0x5	/* unix thread (includes a stack) */
#define	LC_LOADFVMLIB	0x6	/* load a specified fixed VM shared library */
#define	LC_IDFVMLIB	0x7	/* fixed VM shared library identification */
#define	LC_IDENT	0x8	/* object identification info (obsolete) */
#define LC_FVMFILE	0x9	/* fixed VM file inclusion (internal use) */
#define LC_PREPAGE      0xa     /* prepage command (internal use) */
#define	LC_DYSYMTAB	0xb	/* dynamic link-edit symbol table info */
#define	LC_LOAD_DYLIB	0xc	/* load a dynamically linked shared library */
#define	LC_ID_DYLIB	0xd	/* dynamically linked shared lib ident */
#define LC_LOAD_DYLINKER 0xe	/* load a dynamic linker */
#define LC_ID_DYLINKER	0xf	/* dynamic linker identification */
#define	LC_PREBOUND_DYLIB 0x10	/* modules prebound for a dynamically */
				/*  linked shared library */
#define	LC_ROUTINES	0x11	/* image routines */
#define	LC_SUB_FRAMEWORK 0x12	/* sub framework */
#define	LC_SUB_UMBRELLA 0x13	/* sub umbrella */
#define	LC_SUB_CLIENT	0x14	/* sub client */
#define	LC_SUB_LIBRARY  0x15	/* sub library */
#define	LC_TWOLEVEL_HINTS 0x16	/* two-level namespace lookup hints */
#define	LC_PREBIND_CKSUM  0x17	/* prebind checksum */

/*
 * load a dynamically linked shared library that is allowed to be missing
 * (all symbols are weak imported).
 */
#define	LC_LOAD_WEAK_DYLIB (0x18 | LC_REQ_DYLD)

#define	LC_SEGMENT_64	0x19	/* 64-bit segment of this file to be
				   mapped */
#define	LC_ROUTINES_64	0x1a	/* 64-bit image routines */
#define LC_UUID		0x1b	/* the uuid */
#define LC_RPATH       (0x1c | LC_REQ_DYLD)    /* runpath additions */
#define LC_CODE_SIGNATURE 0x1d	/* local of code signature */
#define LC_SEGMENT_SPLIT_INFO 0x1e /* local of info to split segments */
#define LC_REEXPORT_DYLIB (0x1f | LC_REQ_DYLD) /* load and re-export dylib */
#define	LC_LAZY_LOAD_DYLIB 0x20	/* delay load of dylib until first use */
#define	LC_ENCRYPTION_INFO 0x21	/* encrypted segment information */
#define	LC_DYLD_INFO 	0x22	/* compressed dyld information */
#define	LC_DYLD_INFO_ONLY (0x22|LC_REQ_DYLD)	/* compressed dyld information only */
#define	LC_LOAD_UPWARD_DYLIB (0x23 | LC_REQ_DYLD) /* load upward dylib */
#define LC_VERSION_MIN_MACOSX 0x24   /* build for MacOSX min OS version */
#define LC_VERSION_MIN_IPHONEOS 0x25 /* build for iPhoneOS min OS version */
#define LC_FUNCTION_STARTS 0x26 /* compressed table of function start addresses */
#define LC_DYLD_ENVIRONMENT 0x27 /* string for dyld to treat
				    like environment variable */
#define LC_MAIN (0x28|LC_REQ_DYLD) /* replacement for LC_UNIXTHREAD */
#define LC_DATA_IN_CODE 0x29 /* table of non-instructions in __text */
#define LC_SOURCE_VERSION 0x2A /* source version used to build binary */
#define LC_DYLIB_CODE_SIGN_DRS 0x2B /* Code signing DRs copied from linked dylibs */
#define	LC_ENCRYPTION_INFO_64 0x2C /* 64-bit encrypted segment information */
#define LC_LINKER_OPTION 0x2D /* linker options in MH_OBJECT files */
#define LC_LINKER_OPTIMIZATION_HINT 0x2E /* optimization hints in MH_OBJECT files */
#define LC_VERSION_MIN_TVOS 0x2F /* build for AppleTV min OS version */
#define LC_VERSION_MIN_WATCHOS 0x30 /* build for Watch min OS version */

typedef int	vm_prot_t;

struct mach_header {
  uint32_t magic;
  uint32_t cputype;
  uint32_t cpusubtype;
  uint32_t filetype;
  uint32_t ncmds;
  uint32_t sizeofcmds;
  uint32_t flags;
};

struct mach_header_64 {
  uint32_t magic;
  uint32_t cputype;
  uint32_t cpusubtype;
  uint32_t filetype;
  uint32_t ncmds;
  uint32_t sizeofcmds;
  uint32_t flags;
  uint32_t reserved;
};

struct load_command {
	uint32_t cmd;		/* type of load command */
	uint32_t cmdsize;	/* total size of command in bytes */
};


struct segment_command {
  uint32_t  cmd;
  uint32_t  cmdsize;
  char      segname[16];
  uint32_t  vmaddr;
  uint32_t  vmsize;
  uint32_t  fileoff;
  uint32_t  filesize;
  vm_prot_t maxprot;
  vm_prot_t initprot;
  uint32_t  nsects;
  uint32_t  flags;
};

struct segment_command_64 { /* for 64-bit architectures */
	uint32_t	cmd;		/* LC_SEGMENT_64 */
	uint32_t	cmdsize;	/* includes sizeof section_64 structs */
	char		segname[16];	/* segment name */
	uint64_t	vmaddr;		/* memory address of this segment */
	uint64_t	vmsize;		/* memory size of this segment */
	uint64_t	fileoff;	/* file offset of this segment */
	uint64_t	filesize;	/* amount to map from the file */
	vm_prot_t	maxprot;	/* maximum VM protection */
	vm_prot_t	initprot;	/* initial VM protection */
	uint32_t	nsects;		/* number of sections in segment */
	uint32_t	flags;		/* flags */
};

struct section { /* for 32-bit architectures */
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	uint32_t	addr;		/* memory address of this section */
	uint32_t	size;		/* size in bytes of this section */
	uint32_t	offset;		/* file offset of this section */
	uint32_t	align;		/* section alignment (power of 2) */
	uint32_t	reloff;		/* file offset of relocation entries */
	uint32_t	nreloc;		/* number of relocation entries */
	uint32_t	flags;		/* flags (section type and attributes)*/
	uint32_t	reserved1;	/* reserved (for offset or index) */
	uint32_t	reserved2;	/* reserved (for count or sizeof) */
};

struct section_64 { /* for 64-bit architectures */
	char		sectname[16];	/* name of this section */
	char		segname[16];	/* segment this section goes in */
	uint64_t	addr;		/* memory address of this section */
	uint64_t	size;		/* size in bytes of this section */
	uint32_t	offset;		/* file offset of this section */
	uint32_t	align;		/* section alignment (power of 2) */
	uint32_t	reloff;		/* file offset of relocation entries */
	uint32_t	nreloc;		/* number of relocation entries */
	uint32_t	flags;		/* flags (section type and attributes)*/
	uint32_t	reserved1;	/* reserved (for offset or index) */
	uint32_t	reserved2;	/* reserved (for count or sizeof) */
};


namespace macho {

uintptr_t Binary::ConvertVirtualAddress(uintptr_t addr) {
  struct segment_command *seg = (struct segment_command *)SegmentForAddress(addr);
  return addr - seg->vmaddr;
}

uintptr_t Binary::FindMetadataRegistration() {
  return 0; // TODO
}

uintptr_t Binary::FindCodeRegistration() {
  return 0; // TODO
}

bool Binary::IsValid() {
  stream_.set_offset(base_);
  uint32_t magic = stream_.ReadUInt();

  return (magic == MH_MAGIC ||
          magic == MH_CIGAM ||
          magic == MH_MAGIC_64 ||
          magic == MH_CIGAM_64);
}

bool Binary::Is64Bit() {
  stream_.set_offset(base_);
  uint32_t magic = stream_.ReadUInt();
  return (magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}

uintptr_t Binary::FindLoadCommand(uint32_t load_command, bool rebase) {
  if (IsValid()) {
    stream_.set_offset(base_);
    struct load_command *lcmd;
    if (Is64Bit()) {
      mach_header_64 *mh = stream_.View<mach_header_64>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == load_command) {
          return (rebase) ? base_ + (uintptr_t)lcmd : (uintptr_t)lcmd;
        }
      }
    } else {
      mach_header *mh = stream_.View<mach_header>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == load_command) {
          return (rebase) ? base_ + (uintptr_t)lcmd : (uintptr_t)lcmd;
        }
      }
    }
  } else {
    // throw exception
  }
  return 0;
}

uintptr_t Binary::FindSegment(std::string segment, bool rebase) {
  if (IsValid()) {
    const char *segname = segment.c_str();
    stream_.set_offset(base_);
    struct load_command *lcmd;
    if (Is64Bit()) {
      mach_header_64 *mh = stream_.View<mach_header_64>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == LC_SEGMENT_64) {
          struct segment_command_64 *seg64 = (struct segment_command_64 *)(lcmd);
          if (strcmp(seg64->segname, segname) == 0) {
            return (rebase) ? base_ + (uintptr_t)seg64 : (uintptr_t)seg64;
          }
        }
      }
    } else {
      mach_header *mh = stream_.View<mach_header>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == LC_SEGMENT) {
          struct segment_command *seg = (struct segment_command *)(lcmd);
          if (strcmp(seg->segname, segname) == 0) {
            return (rebase) ? base_ + (uintptr_t)seg : (uintptr_t)seg;
          }
        }
      }
    }
  } else {
    // throw exception
  }
  return 0;
}

uintptr_t Binary::FindSection(std::string section, bool rebase) {
  if (IsValid()) {
    const char *sectname = section.c_str();
    stream_.set_offset(base_);
    struct load_command *lcmd;
    if (Is64Bit()) {
      mach_header_64 *mh = stream_.View<mach_header_64>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == LC_SEGMENT) {
          struct segment_command_64 *seg = (struct segment_command_64 *)(lcmd);
          struct section_64 *sect64 = (struct section_64 *)(seg + 1);
          for (uint32_t i=0; i<seg->nsects; i++, sect64++) {
            if (strcmp(sect64->sectname, sectname) == 0) {
              return (rebase) ? base_ + (uintptr_t)sect64 : (uintptr_t)sect64;
            }
          }
        }
      }
    } else {
      mach_header *mh = stream_.View<mach_header>(base_);
      lcmd = (struct load_command *)(mh + 1);
      for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
        if (lcmd->cmd == LC_SEGMENT) {
          struct segment_command_64 *seg = (struct segment_command_64 *)(lcmd);
          struct section *sect = (struct section *)(seg + 1);
          for (uint32_t i=0; i<seg->nsects; i++, sect++) {
            if (strcmp(sect->sectname, sectname) == 0) {
              return (rebase) ? base_ + (uintptr_t)sect : (uintptr_t)sect;
            }
          }
        }
      }
    }
  } else {
    // throw exception
  }
  return 0;
}

uintptr_t Binary::SegmentForAddress(uintptr_t address, bool rebase) {
  stream_.set_offset(base_);
  struct load_command *lcmd;
  if (Is64Bit()) {
    mach_header_64 *mh = stream_.View<mach_header_64>(base_);
    lcmd = (struct load_command *)(mh + 1);
    for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
      if (lcmd->cmd == LC_SEGMENT_64) {
        struct segment_command_64 *seg = (struct segment_command_64 *)(lcmd);
        if (seg->vmaddr <= address && seg->vmaddr + seg->vmsize >= address) {
          return (rebase) ? base_ + (uintptr_t)seg : (uintptr_t)seg;
        }
      }
    }
  } else {
    mach_header *mh = stream_.View<mach_header>(base_);
    lcmd = (struct load_command *)(mh + 1);
    for (uint32_t i=0; i<mh->ncmds; i++, lcmd += (lcmd->cmdsize / sizeof(struct load_command))) {
      if (lcmd->cmd == LC_SEGMENT) {
        struct segment_command *seg = (struct segment_command *)(lcmd);
        if (seg->vmaddr <= address && seg->vmaddr + seg->vmsize >= address) {
          return (rebase) ? base_ + (uintptr_t)seg : (uintptr_t)seg;
        }
      }
    }
  }
  return 0; // throw exception?
}

}
