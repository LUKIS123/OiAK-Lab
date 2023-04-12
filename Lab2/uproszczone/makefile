all: dodawanie odejmowanie

dodawanie odejmowanie:
		as --32 --gstabs $@.s -o $@.o
		ld -m elf_i386 $@.o -o $@
		rm $@.o

clean: dodawanie odejmowanie
		rm dodawanie odejmowanie