# Computer Organization and Architecture – Lab 

This repository contains lab assignments focused on low-level programming, system architecture, and numerical computation using C and x86 assembly. 
Each lab explores a specific aspect of computer organization, such as binary arithmetic, floating-point operations, SIMD optimization, and performance benchmarking.


## Lab 2 – Multi-word Arithmetic in Assembly

Implementation of basic arithmetic operations (addition, subtraction) on large integers represented in binary using x86 assembly. 
The lab focuses on carry propagation and stack-based tracking of intermediate and overflow results.

---

## Lab 3 – Floating-Point Exceptions and IEEE-754

Development of a floating-point calculator in C/ASM with configurable precision and rounding modes. 
The program triggers and analyzes IEEE-754 exceptions (NaN, ±0, ±INF) through GDB.

---

## Lab 4 – SIMD vs SISD Vector Arithmetic

Comparison of SIMD and SISD approaches to vector math (addition, subtraction, multiplication, division) using inline assembly and GCC. 
Includes performance measurement across different input sizes and result visualization with plots.

---

## Lab 5 – Performance Scaling with SIMD Intrinsics

Extends Lab 4 by implementing vector scaling using both SISD and SIMD (SSE intrinsics) in C. Performance data is collected and analyzed to 
evaluate acceleration and SIMD efficiency.

---

## Tools & Technologies

- x86 Assembly
- C language with inline assembly
- SIMD 
- SISD 
- GDB debugger
- GCC toolchain
- Performance measurement

