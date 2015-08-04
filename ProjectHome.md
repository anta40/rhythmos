RhythmOS is a small UNIX-like kernel that currently only runs on x86. The major goal of this project is to provide a highly configurable kernel that can be setup with only the functionality that is needed.

RhythmOS can be easily run virtually with the use of tools like [QEMU](http://qemu.org/).

# Features #
  * Small C library
  * Kernel mode and mode user
  * Basic Multitasking through the use of context switches and memory protection
  * Dynamic memory allocation (using [buddy allocation](http://en.wikipedia.org/wiki/Buddy_memory_allocation) technique)
  * Virtual memory
  * Kernel Paging
  * UNIX-like system calls:
    * Process management: fork, kill, execve, waitpid ...
    * Interprocess Communication (IPC): pipe, dup
  * Read only filesystem