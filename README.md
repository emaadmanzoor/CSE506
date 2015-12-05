------------
CockroachOS 
------------
Group Members: Rachee Singh, Emaad Ahmed Manzoor

--------
Features
--------
* 64-bit, 4 level paging
* Preemption of user processes
* Copy On Write 
* Auto-growing stack
* SEGV handling
* Round robin shceduler
* Binaries for execution: kill, sleep, ls, ps, echo 
* Background processes with "&"
* Ctrl+C for kill
* Auto-scrolling terminal

-------
Testing
-------
Here are a set of binaries that be used to evaluate the features of our OS:
* The ps command prints all running/runnable/sleeping processes in the system.
* A binary named "infinite" that enters an infinite loop. This process can be started in the background with
  "infinite &". The ps command would show that it is executing and it can be killed by calling "kill -9 <pid>"
* Execution of the "infinite" binary can be used to test preemption. The process can
  be placed in the background and can be killed using Ctrl-C (when in foreground).
* "echo <string>" can be used for printing strings on the screen.
* "sleep <seconds>" will block until <seconds> expire and then the shell prompt is returned.

----------------
Acknowledgements
----------------
* The following design principles were inspired from the Xv6 book (https://pdos.csail.mit.edu/6.828/2014/xv6/book-rev8.pdf).
  These include:
  * The kernel stack structure of the first process (kernel context and user context placed on the kernel stack)
  * Context switching mechanism (swtch function)
* User space memory allocation (malloc) implementation inspired from The C Programming Language by Kernighan & Ritchie
* Interrupt Table (ints.s) based on the ideas from https://littleosbook.github.io/.
* James Molloy's OS dev tutorial series: http://www.jamesmolloy.co.uk/tutorial_html/index.html
* OS Dev wiki and forums (osdev.org)
