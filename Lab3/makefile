all: kalkulator

kalkulator:
		as --32 --gstabs $@.s -o $@.o
		ld -m elf_i386 $@.o -o $@
		rm $@.o

clean: kalkulator
		rm kalkulator
