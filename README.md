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

