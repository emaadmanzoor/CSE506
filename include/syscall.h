#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

long (__syscall)(long, ...);

static __inline int64_t syscall_0(uint64_t n) {
	return 0;
}

static __inline int64_t syscall_1(uint64_t n, uint64_t a1) {
        unsigned long ret;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

static __inline int64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) {
	return 0;
}

static __inline int64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3) {
	return 0;
}

#endif
