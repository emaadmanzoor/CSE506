/* Source: MUSL (musl/arch/x86_64/syscall_arch.h)
 *
 * ABI: http://www.x86-64.org/documentation/abi.pdf
 * Section A.2.1 contains details of
 *   - which registers the userspace program populates
 *   - which registers to populate before calling syscall
 */
 
.global __syscall
__syscall:
  movq %rdi,%rax      # arg1: syscall number in rax
  movq %rsi,%rdi      # arg2 in rdi
  movq %rdx,%rsi      # arg3 in rsi
  movq %rcx,%rdx      # arg4 in rdx
  movq %r8,%r10       # arg5 in r10
  movq %r9,%r8        # arg6 in r8
  #movq 8(%rsp),%r9   # arg7 (from the stack) in r9
  #syscall            # we don't use the fast syscall interface
  int $64             # return value should be in rax
  ret
