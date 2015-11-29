#define KSTACKSIZE 4096
#define NPROC 64

struct context {
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rax;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rip;
};

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct proc {
  uint64_t sz;                     // Size of process memory (bytes)
  pte_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  int killed;                  // If non-zero, have been killed
  //struct file *ofile[NOFILE];  // Open files
  //struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};
struct trapframe {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rbp;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  uint64_t trapno;
  uint64_t err;
  uint64_t rip;
  uint64_t rsp;
  uint64_t ss;
  uint64_t cs;
  uint64_t ds;
  uint64_t eflags;
};
