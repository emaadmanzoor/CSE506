#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

/* Inline ASM source: MUSL (musl/arch/x86_64/syscall_arch.h)
 *
 * ABI: http://www.x86-64.org/documentation/abi.pdf
 * Section A.2.1 contains details of
 *   - which registers the userspace program populates
 *   - which registers to populate before calling syscall
 *
 * Implemented for 7 arguments (6 register, 1 stack) in assembly,
 * the function would look like this:
 *
 * .global __syscall
 * __syscall:
 *   movq %rdi,%rax      # arg1: syscall number in rax
 *   movq %rsi,%rdi      # arg2 in rdi
 *   movq %rdx,%rsi      # arg3 in rsi
 *   movq %rcx,%rdx      # arg4 in rdx
 *   movq %r8,%r10       # arg5 in r10
 *   movq %r9,%r8        # arg6 in r8
 *   movq 8(%rsp),%r9    # arg7 (from the stack) in r9
 *   syscall
 *   ret
 *
 * The functions below implement variants of the above for
 * different number of arguments.
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
