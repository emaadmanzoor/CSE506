<<<<<<< Updated upstream
# CSE 506

## Essentials

**Clone a branch**

`git clone -b $BRANCHNAME git@github.com:emaadmanzoor/CSE506.git`

**Make Changes to Our Code**

   * Checkout a new branch: `git checkout -b new-branch`
   * Or go to an existing branch: `git checkout existing-branch`
   * Check which branch you are on: `git status`
   * Delete a branch: `git branch -d existing-branch`
   * Make changes to the code.
   * Push changes: `git push`

**Setup and Pull from Ferdman's Code**

```
git remote add ferdman emanzoor@sbrocks.cewit.stonybrook.edu:/home/facfs1/mferdman/f15.git
git pull ferdman $BRANCHNAME
```

**Setup and Pull from Our Code**

```
git remote add origin git@github.com:emaadmanzoor/CSE506.git
git pull origin $BRANCHNAME
```

**Push to Our Code**

`git push origin $BRANCHNAME`

**Push to Ferdman's Code**

You can't.

## Projects

### Warmup Project 1: Part 1

**Branch name:** `w1p1`

### Warmup Project 1: Part 2

**Branch name:** `w1p2`

## Useful Commands

```
valgrind rootfs/bin/sbush --track-origins=yes --leak-check=full -v
```

```
gdb -ex 'target remote localhost:9998' -ex 'break reload_gdt' -ex 'continue' ./kernel
gdb -ex 'target remote localhost:9998' -ex 'set confirm off' -ex 'file rootfs/bin/hello' -ex 'b malloc' ./kernel
```

```
qemu-system-x86_64 -curses -cdrom $USER.iso -drive id=disk,file=$USER.img,if=none -device ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 -net nic -net user,hostfwd=tcp::10080-:80 -net user,hostfwd=tcp::10023-:23 --no-reboot -gdb tcp::9998
```

## Team

   * Rachee Singh (racsingh@cs.stonybrook.edu)
   * Emaad Ahmed Manzoor (emanzoor@cs.stonybrook.edu)
=======
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

-------
Testing
-------
* The ps command prints all running/runnable/sleeping processes in the system.
* "infinite" binary has an infinite loop. Execution of the binary can be used to test preemption. The process can
  be placed in the background and can be killed using Ctrl-C (when in foreground).

----------------
Acknowledgements
----------------
* Some design principles were inspired from the 32-bit open-source OS, xv6.
>>>>>>> Stashed changes
