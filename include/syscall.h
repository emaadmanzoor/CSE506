#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

int __syscall(int, ...);

/* Inline ASM source: MUSL (musl/arch/x86_64/syscall_arch.h)
 */

static __inline int64_t syscall_0(uint64_t n) {
  unsigned long ret;
  __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
  return ret;
}

static __inline int64_t syscall_1(uint64_t n, uint64_t a1) {
  unsigned long ret;
  __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
  return ret;
}

static __inline int64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
  unsigned long ret;
  __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2) : "rcx", "r11", "memory");
  return ret;
}

static __inline int64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
  unsigned long ret;
  __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
  return ret;
}

static __inline int64_t syscall_4(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
  unsigned long ret;
  register long r10 __asm__("r10") = a4;
  __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10): "rcx", "r11", "memory");
  return ret;
}

#endif
