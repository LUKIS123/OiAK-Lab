all: hello writeText compareText

hello writeText compareText:
	as -o $@.o $@.s
	ld -o $@ $@.o
	rm $@.o

clean: hello writeText compareText
	rm hello writeText compareText