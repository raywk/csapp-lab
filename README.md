# csapp-lab

This is a repo contains my approach to csapp self-study labs and notes for the course 15-213 lecture\reading.

Resource:

[15-213/15-513 Introduction to Computer Systems](https://www.cs.cmu.edu/%7E213/schedule.html)

[CS:APP3e Lab Assignments](https://csapp.cs.cmu.edu/3e/labs.html)

[《深入理解计算机系统》中文电子版](https://hansimov.gitbook.io/csapp)

[TOC]

## notes

### Machine Prog: Basics

![05def](/img/05def.png)
![05reg](/img/05reg.png)

Memory Addressing Modes:
![05mem_addr](/img/05mem_addr.png)

leaq *Src*, *Dst*

- *Src* is address mode expression
- Set *Dst* to address denoted by expression

It does not access the memory, just loads address to *Dst*.

Some Arithmetic Operations:

![05arith_ops](/img/05arithmetic_ops.png)

### Machine Prog: Control

Information about currently executing program

- Temporary data: (`%rax`, ...)
- Location of runtime stack: (`%rsp`)
- Location of current code control point: (`%rip`, ...)
- Status of recent test: (CF, ZF, SF, OF)

Notice: 4 bytes computations will set high 32 bits of registers to be zero, it is a rule. And 2 bytes computations do not have such rule.

Condition Codes (Implicit Setting)

![06concodes](/img/06concodes.png)
CF set when *Carry* in add operations and *Borrow* in **sub** operations.
![06CF](/img/06CF.png)

Jump instructions
![06jump](/img/06jump.png)

SetX instructions
![06setx](/img/06setx.png)
SetX argument is always a low byte (`%al`, `%r8b`, etc.)

Conditional move instructions.
![06condmove](/img/06condmove.png)

`Test` instruction executes logical AND for two operands, then set flags by the result.
`CF` and `OF` should always set to 0.

### Machine Prog: Procedures

The design choices of mechanisms in procedures make up the **Application Binary Interface (ABI)**.

#### Stack Structure

`push` and `pop`
![07push](/img/07push.png)

![07pop](/img/07pop.png)

#### Calling Conventions

- Passing control
- Passing data
- Managing local data

![07pcf](/img/07pcf.png)

![07dataflow](/img/07dataflow.png)

Stack allocated in *Frames*: state for single procedure instantiation, stores

- Arguments
- Local variables
- Return pointer

![07stackframe](/img/07stackframe.png)

Register Saving Conventions
*"Caller Saved"* v.s. *"Callee Saved"*

![07reg_use_1](/img/07reg_use_1.png)

![07reg_use_2](/img/07reg_use_2.png)

### Machine Prog: Data

#### Arrays

![08ptr_and_arr](/img/08ptr_and_arr.png)

*Nested* array vs *Multi-level* array
![08nested_vs_multilevel](/img/08nested_vs_multilevel.png)

2-D matrix
![08nxnmatrix](/img/08nxnmatirx.png)

2-D variable-size matrix
![08vsa](/img/08vsa.png)

#### Structures

*Fields* ordered according to declaration order, even if a more compact ordering exists.

Compiler determines overall size and positions of fields.

An example of linked list:
![08linkedlist](/img/08linkedlist.png)

*Alignment*:
![08align](/img/08align.png)

![08alignexample](/img/08alignexample.png)

For arrays of structures: no padding between array elements.

To save space, put large data types first.
![08save_space](/img/08save_space.png)

#### Floating Point Basics

Arguments passed in `%xmm0`, `%xmm1`, etc.
Result returned in `%xmm0`, and all XMM registers are Caller Saved.

![08fp](/img/08fp.png)

### Machine Prog: Advanced

#### Memory Layout

![09mem_layout](/img/09mem_layout.png)

#### Buffer Overflow

*buffer overflow*: exceed the memory size allocated for an array.

Mostly caused by unchecked lengths on string inputs.

Take care of string libary code:
`gets`: get string.
`strcpy`, `strcat`: Copy strings of arbitrary length.
`scanf`, `fscanf`, `sscanf`, when given `%s` conversion specification.

The EOF `\0` itself also occupies a char.

![09stacksmash](/img/09stacksmash.png)

![09codeinject](/img/09codeinject.png)

##### Avoid Overflow

Use:
`fgets` instead of `gets`.
`strncpy` instead of `strcpy`.
Don't use `scanf` with `%s` conversion specification: use `fgets` or use `%ns` where `n` is a suitable integer.

System-Level protections include:

1. **Randomized stack offsets**: at start of program, allocate random amount of space on stack and shifts stack addresses for entire program.
2. **Non-executable memory**: x86-64 added a way to mark regions of memory as *not executable*, programs will crash on jumping into any such region.
3. **Stack canaries**: place special value ("canary") on stack just beyond buffer and check for corruption before exiting function.

for canary check:
usually use `xor %fs:0x28,%rax` compare to canary.

##### Return-Oriented Programming Attacks

Stack randomization and marking stack non-executable makes it hard to predict buffer location and to insert binary code.

Alternative strategy is to use existing code.

`TODO`

#### Unions

![09union](/img/09union.png)

### Design and Debugging

### The Memory Hierarchy

![10bus](/img/10bus.png)

#### RAM

RAM (Random-Access Memory) is the main memory building block, basic storage unit is normally a *cell* (one bit per cell).
System "main memory" comprises multiple RAM chips.

Two main varieties of RAM: SRAM (Static RAM), DRAM (Dynamic RAM).

*DRAM*: 1 transistor + 1 capacitor per bit, must refresh state periodically.
*SRAM*: 6 transistor per bit, holds state indefinitely.

![10readDRAM](/img/10readDRAM.png)
one block is one *supercell*

![10DRAM](/img/10DRAM.png)

#### Locality

![10locality](/img/10locality.png)

![10localityexample](/img/10localityexample.png)

#### Memory Hierarchy

Some properties of hardware and software:

- Fast storage costs more per byte and has less capacity.
- The gap between CPU and main memory speed is widening (CPU is getting faster and faster).
- Well-written programs tend to exhibit good locality.

These properties lead to an approach for organizing memory and storage systems known as a *memory hierarchy*.

![10memoryhierarchy](/img/10memoryhierarchy.png)

*Cache*: A smaller, faster storage device that acts as a staging area for a subset of the data in a larger slower device.

Why cache?
speed gap of two near storage layer + locality of programs

![10cachevsmem](/img/10cachevsmem.png)

placement policy and replacement policy.

Cache Misses
![10cachemiss](/img/10cachemiss.png)

#### Storage technologies and trends

Storage tech:

- Magnetic Disks
- Nonvolatile (Flash) Memory

![10disk](/img/10disk.png)

Disk access time = seek time + rotational latency + transfer time

![10diskaccesstime](/img/10diskaccesstime.png)

direct memory access (DMA)

![10rom](/img/10rom.png)

![10ssd](/img/10ssd.png)

Synchronous DRAM (*SDRAM*): uses a conventional clock signal instead of asynchronous control.

Double data-rate synchronous DRAM (*DDR SDRAM*):

- Double edge clocking sends two bits per cycle per pin
- Size of small perfetch buffer: DDR (2 bits), DDR2 (4 bits), DDR3 (8 bits), DDR4 (16bits), DDR5

### Cache Memories

#### Cache memory organization and operation

![11concepts](/img/11concepts.png)

![11set-asso_cache_org](/img/11set-asso_cache_org.png)

![11cache_read](/img/11cache_read.png)

![11direct_mapped_cache](/img/11direct_mapped_cache.png)

![11direct_mapped_cache_sim](/img/11direct_mapped_cache_sim.png)

![11e-way_set_asso_cache](/img/11e-way_set_asso_cache.png)

![11two-way_set_asso_cache_sim](/img/11two-way_set_asso_cache_sim.png)

![11cachewrite](/img/11cachewrite.png)

If a line is evicted and dirty bit is set to 1, the entire block of $2^b$ bytes are written back to memory.

Why index using middle bits?

If use high bits indexing, then near addresses will be mapped into the same set.

Why 99% hits is twice as good as 97%?

97% hits: 1 cycle + 0.03 x 100 cycles = 4 cycles

99% hits: 1 cycle + 0.01 x 100 cycles = 2 cycles

This is why "miss rate" is used instead of "hit rate".

#### Performance impact of caches

 *Read throughput* (read bandwidth): number of bytes read from memory per second (MB/s)

![11mem_mountain](/img/11mem_mountain.png)

![11matmul](/img/11matmul.png)

![11block](/img/11block.png)

No blocking: $(9/8) n^3$ misses

Blocking: $(1/(4B)) n^3$ misses

Use largest block size $B$, such that $B$ satisfies $3B^2 < C$.

### Code Optimization

### Linking

Why Linkers?

*Modularity*:

Programs can be written as a collection of smaller source files, rather than one monolithic mass.

*Efficiency*:

Time: separate compilation

Space: libraries

- *static linking*: executable files and running memory images contain only the library code they actually use.
- *dynamic linking*: during execution, single copy of library code can be shared across all executing processes.

What do linkers do?

Symbol resolution

*symbols*: global variables and functions.

```c
void swap() {...} /* define symbol swap */
swap()            /* reference symbol swap */
int *xp = &x;     /* define symbol xp, reference x */
```

Symbol definitions are stored in object file (by assembler) in *symbol table*, which is an array of entries, each entry includes name, size and location of symbol.

Relocation

Merges separate code and data sections into single sections.

Relocates symbols from their relative locations in the `.o` files to their final absolute memory locations in the executable.

Updates all references to these symbols to reflect their new positions.

![13object_files](/img/13object_files.png)

![13elf1](/img/13elf1.png)

![13elf2](/img/13elf2.png)

![13linker_symbols](/img/13linker_symbols.png)

Local non-static C variables stored on the stack.

Local static C variables stored in either `.bss` or `.data`.

Create local symbols in the symbol table with unique names, e.g., `x`, `x.1721` and `x.1724`.

*Strong symbols*: procedures and initialized globals.

*Weak symbols*: uninitialized globals or ones declared with specifier `extern`.

![13symbol_rules](/img/13symbol_rules.png)

example of `extern` in `.h` files

![13extern](/img/13extern.png)

Relocation:

![13relocation](/img/13relocation.png)

`TODO: relocation rules`

![13executable](/img/13executable.png)

### Virtual Memory: Concepts

![14vm_concepts](/img/14vm_concepts.png)

*Linear address space*: Ordered set of contiguous non-negative integer addresses: $\{0, 1, 2, 3 \dots\}$.

*Virtual address space*: Set of $N = 2^n$ virtual addresses: $\{0, 1, 2, 3, \dots, N-1\}$.

*Physical address space*: Set of $M = 2^m$ physical addresses: $\{0, 1, 2, 3, \dots, M-1\}$.

Why virtual memory?

1. uses main memory efficiently (use DRAM as a cache for parts of a virtual address space).
2. simplifies memory management: each process gets the same uniform linear address space.
3. isolate address space.

### Virtual Memory: Systems

`TODO: lec15`

### Dynamic Memory Allocation: Basic

`TODO: lec16`

### Dynamic Memory Allocation: Advanced

`TODO: lec17`

### ECF: Processes and Multitasking

#### Processes

Definition: A *process* is an instance of a running program.

Process provides each program with two key abstractions:

- Private address space
- Logical control flow

From startup to shutdown, each CPU core simply reads and executes a sequence of machine instructions, one at a time, this sequence is the CPU's *control flow*.

Control flow passes from one process to another via a *context switch*.

#### System Calls

syscall example:

- read/write files
- get current time
- allocate RAM (sbrk)

Almost all system-level operations can fail. On error, most system-level functions retuan -1 and set global variable `errno` to indicate cause.

![18error_handling](/img/18error_handling.png)

#### Process Control

Obtaining process IDs:

```c
pid_t getpid(void); // return PID of current process

pid_t getppid(void); // return PID of parent process
```

Process states:

- Running: either executing or *could be* executing if there were enough CPU cores.
- Blocked/Sleeping: cannot execute until some external event happens (usually I/O).
- Stopped: has been prevented from executing by user action (Ctrl + Z).
- Terminated/Zombie: process is finished and parent process has not yet been notified.

*Parent process* creates a new runnig *child process* by calling *fork*.

```c
int fork(void);
```

returns 0 to the child process, child's PID to parent process.

called *once* but returns *twice*.

![18fork](/img/18fork.png)

![18reap_child](/img/18reap_child.png)

If child process exits without notifying the parent process, then it becomes a "zombie", make sure `wait` or `waitpid` has been used to terminate child.

![18wait](/img/18wait.png)

关于 `waitpid`:

![18waitpid](/img/18waitpid.png)

`wait(&status)` is equivalent to `waitpid(-1, &status, 0)`.

![18execve](/img/18execve.png)

![18execve_example](/img/18execve_example.png)

![18execve_stack](/img/18execve_stack.png)

![18execve_memory_layout](/img/18execve_memory_layout.png)

#### Shells

A *shell* is an application program that runs programs on behalf of the user

![18shell_example](/img/18shell_example.png)

### ECF: Exceptional Control Flow

Up to now: two mechanisms for changing control flow:

- jumps and branches
- call and return

react to changes in *program state*

Insufficient for a useful system: difficult to react to changes in *system state*

- data arrives from a disk or a network adapter
- instruction divides by zero
- user hits Ctrl-C at the keyboard
- system timer expires

That is why system needs mechanisms for "exceptional control flow".

![19ecf](/img/19ecf.png)

#### Exceptions

An *exception* is a transfer of control to OS *kernel* in response to some *event*.

- Kernel is the memory-resident part of the OS
- Examples of events: divide by 0, arithmetic overflow, page fault, I/O request completes, typing Ctrl-C

there is an exception table in memory

- Asynchronous ECF
  - Interrupts
- Synchronous ECF
  - Traps
  - Faults
  - Aborts

![19asyn_ecf](/img/19asyn_ecf.png)

![19syn_ecf](/img/19syn_ecf.png)

![19syscall](/img/19syscall.png)

#### Signals

ECF exists at all levels of a system

- exceptions: hardware and operating system kernel software
- process context switch: hardware timer and kernel software
- signals: kernel software and application software
- nonlocal jumps: application code

The kernel will interrupt regular processing to alert us when a background process completes. In Unix, the alert mechanism is called a *signal*.

![19signals](/img/19signals.png)

Kernel *sends* a signal to a *destination process* by updating some state in the context of the destination process.

![19receive_signal](/img/19receive_signal.png)

![19pending/blocked_signal](/img/19pending_and_blocked_signal.png)

Kernel maintains `pending` and `blocked` bit vectors in the context of each process

- `pending`: represents the set of pending signals
  - Kernel sets bit k in `pending` when a signal of type k is sent
  - Kernel clears bit k in `pending` when a signal of type k is received
- `blocked`: represents the set of blocked signals
  - Can be set and cleared by using the `sigprocmask` function
  - Also referred to as the `signal mask`

![19process_group](/img/19process_group.png)

Typing ctrl-c (ctrl-z) causes the kernel to send a SIGINT (SIGTSTP) to every job in the foreground process group

- SIGINT - default action is to **terminate** each process
- SIGTSTP - default action is to stop (**suspend**) each process

Kernel computes `pnb = pending & ~blocked`

![19receive_signal_2](/img/19receive_signal_2.png)

Each signal type has a predefined *default action*, which is one of:

- The process terminates
- The process stops until restarted by a SIGCONT signal
- The process ignores the signal

进程可以通过使用 signal 函数修改和信号相关联的默认行为。唯一的例外是 SIGSTOP 和 SIGKILL，它们的默认行为是不能修改的。

![19signal_handler](/img/19signal_handler.png)

**隐私阻塞机制**：主程序捕获到信号 s，该信号会中断主程序，将控制转移到处理程序 S。S 在运行时，程序捕获信号 t≠s，该信号会中断 S，控制转移到处理程序 T。当 T 返回时，S 从它被中断的地方继续执行。最后，S 返回，控制传送回主程序，主程序从它被中断的地方继续执行。
![19nested_signal_handler](/img/19nested_signal_handler.png)

**显式阻塞机制**：应用程序可以使用 sigprocmask 函数和它的辅助函数，明确地阻塞和解除阻塞选定的信号。
![19blocking_and_unblocking_signal](/img/19blocking_and_unblocking_signal.png)

```c
#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
//返回；如果成功则为 0，若出错则为 -1。

int sigismember(const sigset_t *set, int signum);
// 返回：若 signum 是 set 的成员则为 1，如果不是则为 0，若出错则为 -1。
```

sigprocmask 函数改变当前阻塞的信号集合。具体的行为依赖于 how 的值：

- SIG_BLOCK: 把 set 中的信号添加到 blocked 中 (blocked = blocked | set)。
- SIG_UNBLOCK: 把 blocked 中删除 set 中的信号 (blocked = blocked & ~set)。
- SIG_SETMASK: block = set。

![19how_to_write_handler](/img/19how_to_write_handler.png)

signal 处理流程

![19signal_handler_2](/img/19signal_handler_2.png)

Function is *async-signal-safe* if either reentrant or non-interruptible by signals.

on the list:

`_exit`, `write`, `wait`, `waitpid`, `sleep`, `kill`

not on the list:

`printf`, `sprintf`, `malloc`, `exit`

信号的一个与直觉不符的方面是未处理的信号是不排队的。因为 `pending` 位向量中每种类型的信号只对应有一位，所以每种类型最多只能有一个未处理的信号。因此，如果两个类型 k 的信号发送给一个目的进程，而因为目的进程当前正在执行信号 k 的处理程序，所以信号 k 被阻塞了，那么第二个信号就简单地被丢弃了，它不会排队。

![19incorrect_signal_handling](/img/19incorrect_signal_handling.png)

![19correct](/img/19correct.png)

正确使用 `sigprocmask` 函数和它的辅助函数消除竞争

![19correct2](/img/19correct2.png)

Explicitly waiting for signals

![19explicitly_waiting_for_signals](/img/19explicitly_waiting_for_signals.png)

use `sigsuspend`:

```c
int sigsuspend(const sigset_t *mask);

// Equivalent to atomic version of:
sigprocmask(SIG_SETMASK, &mask, &prev);
pause();
sigprocmask(SIG_SETMASK, &prev, NULL);
```

![19sigsuspend](/img/19sigsuspend.png)

#### Nonlocal Jumps

![19nonlocal_jumps](/img/19nonlocal_jumps.png)

![19nonlocal_jumps_2](/img/19nonlocal_jumps_2.png)

`TODO: examples`

## labs

### datalab

一些位操作的魔法：

```c
/*
* if x = 0, then y = 0,
* otherwise y = 0x1.
*/
int y = !!x;
```

```c
/* 
* y = 0 -> z = 0;
* y = 0x1 -> z = 0xffffffff;
* use this to generate mask!
*/
int z = ~y + 1;
```

区分 `-1` 和 `t_max`, `t_min`:
|$x$|$\sim x$|$!(\sim x)$|
|:---:|:---:|:---:|
|$-1$|$0$|$1$|
|$t_{max}$|$t_{min}$|$0$|
|$t_{min}$|$t_{max}$|$0$|

对于 `t_max` 和 `t_min`

|$x$|$(x+1)\oplus(\sim x)$|
|:---:|:---:|
|$t_{max}$|$0$|
|$t_{min}$|$-2$|

## misc

### gdb

![misc_gdb](/img/misc_gdb.png)
