.global _start

.section .data

 zero: .float 0.0
 minus_one: .float -1.0
 two: .float 2.0
 one: .float 1.0
 number: .float 6.0
 numberA: .float 2.5

single_precision: .word 0x07F
double_precision: .word 0x27F

# round single
 down: .word 0x47F
 up: .word 0x87F
 toward: .word 0xC7F

# round double
 down_d: .word 0x6FF
 up_d: .word 0xAFF
 toward_d: .word 0xFFF
 
.section .text

_start:

 finit

# precyzja
 fldcw single_precision
 fldcw double_precision
 
 exceptions:
 # +0
 fldz

 # -0 (0.0/-1.0)
 fldz
 fmuls minus_one

 # +inf (1.0/0.0)
 fld1
 fdivs  zero

 # -inf (-1.0/0.0)
 flds minus_one
 fdivs zero

 # NaN (0.0/0.0)
 fldz
 fdivs zero

free_registers1:
 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)


 arythmetic_operations:
 flds number
 fadds one

 # odejmowanie
 flds number
 fsubs one

 # mnożenie
 flds number
 fmuls two

 # dzielenie
 flds number
 fdivs two


free_registers:
 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

 ffree %st(0)
 fstp %st(0)

round_single:
 fldcw up
 fld numberA
 fadd one
 
round_double:
 fldcw up_d
 fld numberA
 fadd one

exit:
 mov $1, %eax
 xor %ebx, %ebx
 int $0x80
