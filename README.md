# csapp-lab

This is a repo contains my approach to csapp self-study labs and notes for the course 15-213 lecture\reading.

Resource:
[15-213/15-513 Introduction to Computer Systems](https://www.cs.cmu.edu/%7E213/schedule.html)

[CS:APP3e Lab Assignments](https://csapp.cs.cmu.edu/3e/labs.html)

[TOC]

## datalab

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
