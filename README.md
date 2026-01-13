# Embedded Linux Coursework

This repository contains embedded Linux and systems programming coursework developed on a Raspberry Pi platform as part of an Operating Systems for Embedded Applications course.

The projects demonstrate practical experience with C programming, POSIX APIs, process control, scheduling algorithms, multithreading, synchronization, and Linux kernel modules.

All code was written and tested on embedded Linux (Raspberry Pi 5) using GCC, Makefiles, and standard Linux development tools.

---

## Platform

- Hardware: Raspberry Pi 5  
- OS: Embedded Linux (Ubuntu / Debian-based)  
- Language: C  
- Toolchain: GCC, Make, SSH, VS Code Remote  
- APIs: POSIX, pthreads, Linux kernel APIs  

---

## Projects

---

### Skeleton Shell

A simple command-line shell implementing:

- Command parsing and argument tokenization  
- Process creation using `fork()`  
- Program execution using `execvp()`  
- Parent/child synchronization using `waitpid()`  
- Built-in commands and input handling  

Directory: `/skeleton_shell`

---

### Lottery / Stride Scheduler Simulator

User-space simulator modeling CPU scheduling fairness and timing behavior.

- Process structures with tickets and stride calculation  
- Sorted run queue management  
- Context switch overhead modeling  
- Execution time accounting and utilization analysis  
- Deterministic simulation output  

Directory: `/scheduler_lottery`

---

### Multithreading and Synchronization

Demonstrations of race conditions and mutual exclusion using POSIX threads.

- Shared data access across multiple threads  
- Mutex-based critical section protection  
- Analysis of atomicity and scheduling behavior  
- Thread lifecycle management using `pthread_create()` and `pthread_join()`  

Directory: `/threads`

---

### Linux Character Device Driver

Linux kernel module implementing a simple character device with a user-space interface.

- Dynamic major number registration  
- Read/write handlers using `copy_to_user()` and `copy_from_user()`  
- IOCTL command handling  
- Device node creation and permissions  
- Kernel module build system using Makefiles  
- User-space test application for device interaction  

Directory: `/kernel_module`

---

## Skills Demonstrated

- Embedded Linux development  
- C systems programming  
- POSIX process control and IPC  
- Multithreading and synchronization  
- Scheduling algorithms  
- Linux kernel module development  
- Build systems and Makefiles  
- SSH-based remote development  

---

## Contact

Daniel Romero  
Portfolio: https://electricalromero.com  
LinkedIn: https://www.linkedin.com/in/daniel-romero-ee  
