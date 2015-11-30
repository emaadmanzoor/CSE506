#ifndef _TARFS_H
#define _TARFS_H

extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag[1];
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};

// ELF and program headers
// See https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#File_header

#define MAGIC     0x464c457f  //  46 = 'F'; 4c = 'L'; 45 = 'E'; 7F
#define ELF_LOAD  1           // load segment type
#define ELF_R     1           // read flag
#define ELF_W     2           // write flag
#define ELF_X     4           // executable flag
#define BLKSIZE   512         // bytes

// note: 64 bit addresses are little endian
// so 0x0000 0040 0000 0000 is address
// 0x0000 0000 0000 0040. (LSB | MSB)
struct elfheader {
  uint32_t magic;        // ELF magic number
  uint8_t elf_class;     // 32/64 (=2) bit format
  uint8_t endian;        // little (=1) or big endianness
  uint8_t version;       // ELF version (=1)
  uint8_t abi;           // ABI (=0; System V)
  uint8_t abi_version;   // ABI version
  char pad[7];           // padding
  uint16_t type;         // 2 = executable
  uint16_t machine;      // 0x3E = x86-64
  uint32_t elf_version;  // =1
  uint64_t entry;        // process entry address
  uint64_t phoff;        // program header table offset
  uint64_t shoff;        // section header table offset
  uint32_t flags;        // some flags
  uint16_t ehsize;       // size of the ELF header (= 64 bytes)
  uint16_t phentsize;    // size of the program header table entry
  uint16_t phnum;        // number of program header entries (=2 usually)
  uint16_t shentsize;    // size of the section header table entry
  uint16_t shnum;        // number of section header entries
  uint16_t shstrndx;     // index of the sht entry containing sec. names
};

// See http://wiki.osdev.org/ELF
struct progheader {
  uint32_t type;         // type of segment (1 = load, ignore others)
  uint32_t flags;        // flags (RWX = 124)
  uint64_t offset;       // file offset where this segment's data is
  uint64_t vaddr;        // virt mem addr where to place this segment
  uint64_t unused;       // undefined for System V (usually phys addr)
  uint64_t filesz;       // segment size in the file
  uint64_t memsz;        // segment size in memory
  uint64_t align;        // alignment for this section
};

struct elfheader* get_elf_header(char *path);
void map_program_binary(char *, struct proc *);

#endif
