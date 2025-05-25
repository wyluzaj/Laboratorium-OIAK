point: point.o
	ld -m elf_i386 -o point point.o
point.o: point.s
	as --32 --gstabs point.s -o point.o
clean:
	rm -f point.o point
