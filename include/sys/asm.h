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
                        : "=a" (data)
                        : "d" (port)
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

static __inline void ltr(uint16_t p_tr) {
  __asm__ __volatile__ ("ltr %0"
                        : // no output
                        : "r" (p_tr));
}

static __inline void lcr3( uint64_t val)
{
  __asm__ __volatile__("movq %0, %%cr3" : : "r" (val));
}

static __inline uint64_t rcr2()
{
  uint64_t address;
  __asm__ __volatile__("movq %%cr2, %0" : "=r" (address));
  return address;
}
static __inline uint64_t rcs()
{
  uint64_t address;
  __asm__ __volatile__("movq %%cs, %0": "=r" (address));
  return address;
}
#endif
