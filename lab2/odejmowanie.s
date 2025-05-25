.section .data
liczba1:
 .long 0x22222222, 0x22222222, 0x22222222, 0x22222222
liczba2:
 .long 0x11111111, 0x11111111, 0x11111111, 0x11111111

.section .text
.global _start

_start:
 clc
 movl liczba1, %eax
 movl liczba2, %ebx
 subl %ebx, %eax
 pushl %eax

 movl liczba1+4, %eax
 movl liczba2+4, %ebx
 subl %ebx, %eax
 pushl %eax

 movl liczba1+8, %eax
 movl liczba2+8, %ebx
 subl %ebx, %eax
 pushl %eax

 movl liczba1+12, %eax
 movl liczba2+12, %ebx
 subl %ebx, %eax
 pushl %eax

 jnc brak_przeniesienia
 pushl $1

brak_przeniesienia:
 jmp finish

finish:
 movl $1, %eax
 xorl %ebx, %ebx
 int $0x80

