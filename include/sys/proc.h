#include <sys/tarfs.h>

struct context {
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t rbx;
  uint64_t rbp;
  uint64_t rip;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
struct context *kcontext;
struct proc* current_proc;
extern void swtch( struct context **old, struct context *new );
struct proc {
  uint64_t sz;                     // Size of process memory (bytes)
  pte_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct usercontext *ucontext;        // Trap frame for current syscall
  struct context *kcontext;     // swtch() here to run process
  int killed;                  // If non-zero, have been killed
  struct file files[MAX_FILE];
  char cwd[32];
  char name[32];               // Process name (debugging)
  uint64_t startva;
  uint64_t endva;
  uint64_t stackbottom;       // stacktop = KERNBASE
  uint64_t stacktop;
};
