------------
CockroachOS 
------------

*Preemptive OS, 70 points*

Team:
   * Rachee Singh (racsingh@cs.stonybrook.edu)
   * Emaad Ahmed Manzoor (emanzoor@cs.stonybrook.edu)

--------
Features
--------

## Kernel
   * Round-robin preemptive scheduling
   * COW fork
   * Auto-growing stack
   * Graceful SEGV handling

## Userspace

   * Available binaries: `kill`, `sleep`, `ls`, `ps`, `echo`, `cat`, `infinite`, `tests`
   * C standard library
   * Execute background processes with `&`
   * View running processes with `ps`
   * Kill a foreground running process with `^C`
   * Kill a background running process with `kill -9 <pid>`
   * Sleep for `n` seconds with `sleep <n>`
   * Run an infinite loop with `infinite` (useful to test `ps`, `kill` and `^C`)
   * View files in the current directory with `ls`, change directory with `cd` (you can `ls`, but cannot `cd` to `/`!)
   * Print a file to the screen with `cat <filepath>`
   * Clear the screen with `clear`
   * The terminal auto-scrolls!

----------------
Acknowledgements
----------------

   * The mechanism of setting up the first process's stack was adopted from [xv6](https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf) (commentary on a 32-bit open-source OS).
   * User space `malloc` is based on the implementation in K&R's C Programming Language.
   * The interrupt handling mechanism (offloading to a generic handler) is based on the ideas from https://littleosbook.github.io/.
   * Referred standard resources have been cited inline with the code.
