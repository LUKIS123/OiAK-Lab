all: add substract

add substract:
		as --32 --gstabs $@.s -o $@.o 
		ld -m elf_i386 $@.o -o $@
		rm $@.o

clean: add substract
		rm add substract