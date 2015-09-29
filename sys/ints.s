# Based on ideas from https://littleosbook.github.io/:
#   Section 6.4. Creating a Generic Interrupt Handler
#
# Instead of using assembler macros, we plan to write 
# an interrupt handler for each number 0 - 256,
# each of which does pretty much the same thing:
#   - save the error code (or 0 if there is none)
#     ("The specific CPU interrupts that put an error
#       code on the stack are 8, 10, 11, 12, 13, 14
#       and 17." - Section 6.3)
#   - save the interrupt number
#   - call the generic interrupt handler, passing in
#     the interrupt number and error code
#
# (For now the jump table is filled in with zeroes
#  and only handlers for int3 and int32 exist.)

.text

.global handler

# software interrupt for testing
.global int3
.align 16
int3:
  pushq $3
  pushq $0
  jmp handler

# for the timer hardware interrupt
.global int32 
.align 16
int32:
  pushq $32
  pushq $0
  jmp handler

.data

# jump table for interrupt handlers
.global ints
.align 16
ints:
  .skip 8 * 3
  .quad int3
  .skip 8 * (32 - 3 - 1)
  .quad int32 
  .skip 8 * (255 - 32)
