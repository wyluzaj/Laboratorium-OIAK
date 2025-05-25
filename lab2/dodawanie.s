.section .data
liczba1:
    .long 0x11111111, 0x11111111, 0xF1111111, 0x11111111  # liczba1, 4 słowa
liczba2:
    .long 0x11111111, 0x11111111, 0x11111111, 0x11111111  # liczba2, 4 słowa
.section .text
.global _start

_start:
 clc
 movl liczba1, %eax
 movl liczba2, %ebx
 adcl %ebx, %eax
 pushl %eax

 movl liczba1+4, %eax
 movl liczba2+4, %ebx
 adcl %ebx, %eax
 pushl %eax

 movl liczba1+8, %eax
 movl liczba2+8, %ebx
 adcl %ebx, %eax
 pushl %eax

 movl liczba1+12, %eax
 movl liczba2+12, %ebx
 adcl %ebx, %eax
 pushl %eax

 jnc brak_przeniesienia
 pushl $1

brak_przeniesienia:
 jmp finish

finish:
 movl $1, %eax
 xorl %ebx, %ebx
 int $0x80


