# Based on ideas from https://littleosbook.github.io/:
#   Section 6.4. Creating a Generic Interrupt Handler
#
# Instead of using assembler macros, we plan to write 
# an interrupt handler for each number 0 - 256,
# each of which does pretty much the same thing:
#   - save the error code
#     in x86_64, all hardware interrupts generate an error code
#   - save the interrupt number
#   - call the generic interrupt handler, passing in
#     the interrupt number and error code

.text

.global handler

## Hardware (CPU) interrupts (0-31)
.global int0
.align 16
int0:
  pushq $0
  jmp handler

.global int1
.align 16
int1:
  pushq $1
  jmp handler

.global int2
.align 16
int2:
  pushq $2
  jmp handler

.global int3
.align 16
int3:
  pushq $3
  jmp handler

.global int4
.align 16
int4:
  pushq $4
  jmp handler

.global int5
.align 16
int5:
  pushq $5
  jmp handler

.global int6
.align 16
int6:
  pushq $6
  jmp handler

.global int7
.align 16
int7:
  pushq $7
  jmp handler

.global int8
.align 16
int8:
  pushq $8
  jmp handler

.global int9
.align 16
int9:
  pushq $9
  jmp handler

.global int10
.align 16
int10:
  pushq $10
  jmp handler

.global int11
.align 16
int11:
  pushq $11
  jmp handler

.global int12
.align 16
int12:
  pushq $12
  jmp handler

.global int13
.align 16
int13:
  pushq $13
  jmp handler

.global int14
.align 16
int14:
  pushq $14
  jmp handler

## User-defined interrupts (32-255)

# timer interrupt IRQ0
.global int32 
.align 16
int32:
  pushq $0    # dummy error code
  pushq $32
  jmp handler

# keyboard interrupt IRQ1
.global int33
.align 16
int33:
  pushq $0    # dummy error code
  pushq $33
  jmp handler

# system calls
.global int64
.align 16
int64:
  pushq $0    # dummy error code
  pushq $64
  jmp handler

.data

# jump table for interrupt handlers
.global ints
.align 16
ints:
  .quad int0
  .quad int1
  .quad int2
  .quad int3
  .quad int4
  .quad int5
  .quad int6
  .quad int7
  .quad int8
  .quad int9
  .quad int10
  .quad int11
  .quad int12
  .quad int13
  .quad int14
  .skip 8 * (32 - 14 - 1)
  .quad int32 
  .quad int33
  .skip 8 * (64 - 33 - 1)
  .quad int64
  .skip 8 * (255 - 64)
