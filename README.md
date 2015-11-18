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
```

## Team

   * Rachee Singh (racsingh@cs.stonybrook.edu)
   * Emaad Ahmed Manzoor (emanzoor@cs.stonybrook.edu)
