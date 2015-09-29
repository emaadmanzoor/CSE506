#ifndef _ASM_H
#define _ASM_H

#include <sys/defs.h>

// helper functions for assembly commands

/*
 * IA32 manual page 1157: (note Intel ASM syntax)
 *  OUT DX, AL if the command is a byte
 *  OUT DX, AX if the command is a word
 *  OUT DX, EAX if the command is a dword
 * So the port is always in a D register and the
 * command is in an A register.
 */
static __inline void outb(uint16_t port, uint8_t cmd) {
  __asm__ __volatile__ ("out %0, %1"
                        : // no output
                        : "a" (cmd), "d" (port)
                        ); 
}

/*
 * IA32 manual page 927: (note Intel ASM syntax)
 *  IN AL, DX if the data is a byte
 *  IN AX, DX if the data is a word
 *  IN EAX, DX if the data is a dword
 * So the output is always in an A register and the port
 * in a D register.
 */
static __inline uint8_t inb(uint16_t port) {
  uint8_t data;
  __asm__ __volatile__ ("in %1, %0" // reverse so data is first
                        : "=a" (port) 
                        : "d" (data)
                        ); 
  return data;
}

static __inline void sti() {
  __asm__ __volatile__ ("sti");
}

static __inline void lidt(uint64_t p_idtr) {
  __asm__ __volatile__ ("lidt (%0)"
                        : // no output
                        : "r" (p_idtr));
}

#endif
