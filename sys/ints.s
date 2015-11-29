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
  .skip 8 * (32 - 3 - 1)
  .quad int32 
  .quad int33
  .skip 8 * (64 - 33 - 1)
  .quad int64
  .skip 8 * (255 - 64)
