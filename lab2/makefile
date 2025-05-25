all: dodawanie odejmowanie
dodawanie: dodawanie.o
	ld -m elf_i386 -o dodawanie dodawanie.o
dodawanie.o: dodawanie.s
	as --32 --gstabs dodawanie.s -o dodawanie.o
odejmowanie: odejmowanie.o
	ld -m elf_i386 -o odejmowanie odejmowanie.o
odejmowanie.o: odejmowanie.s
	as --32 --gstabs odejmowanie.s -o odejmowanie.o
clean:
	rm -f dodawanie.o odejmowanie.o dodawanie odejmowanie
