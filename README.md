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
* Some design principles were inspired from the 32-bit open-source OS, xv6.
