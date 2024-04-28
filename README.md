# csapp-lab

This is a repo contains my approach to csapp self-study labs and notes for the course 15-213 lecture\reading.

Resource:
[15-213/15-513 Introduction to Computer Systems](https://www.cs.cmu.edu/%7E213/schedule.html)

[CS:APP3e Lab Assignments](https://csapp.cs.cmu.edu/3e/labs.html)

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

