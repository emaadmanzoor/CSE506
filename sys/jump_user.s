.text

# void jump_to_user(uint64_t rip, uint64_t rsp,
#                   uint64_t cs, uint64_t ds, uint64_t flags)
#
# sets up the target stack with argc=0, so that _start
# can launch the main function without problems

.global jump_to_user
.align 16
jump_to_user:
  subq    $8, %rsi      # simulate pushing onto the target stack
  movq    $0, (%rsi)    # place argc = 0 at the top of the stack

                        # set up the current stack before IRET executes
  pushq   %rcx          # [rsp + 32]  ss:     stack segment selector (=ds)
  pushq   %rsi          # [rsp + 24]  rsp:    target (usermode) stack pointer
  pushfq
  popq    %rax          # enable flags in RFLAGS
  orq     %rax, %r8
  pushq   %rax          # [rsp + 16]  rflags: target flags register
  pushq   %rdx          # [rsp +  8]  cs:     code segment selector
  pushq   %rdi          # [rsp +  0]  rip:    target (usermode) instr pointer
  iretq
