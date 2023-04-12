.code32
.text
.global _start
_start:
    # czyszczenie flag
    clc
    pushf
    # wpisanie dlugosci danych do rejestrow
    movl $liczba1_len, %esi
    movl $liczba2_len, %edi

dodawanie:
    subl $4, %esi
    subl $4, %edi

    popf

    movl liczba1(, %esi, 1), %eax
    adcl liczba2(, %edi, 1), %eax

    movl %eax, wynik(, %esi, 1)     # zapisywanie wyniku dodawania do naszej tablicy wynikow
    pushl %eax                      # zapisywanie wyniku dodawania na stosie

    pushf

    cmp $0, %esi
    jz koniec_dodawania

    cmp $0, %edi
    jz uzupelnienie_esi

    jmp dodawanie

uzupelnienie_esi:                   # jesli pierwsza liczba jest dluzsza
    cmp $0, %esi
    jz koniec_dodawania

    subl $4, %esi

    popf

    movl liczba1(, %esi, 1), %eax
    adcl $0, %eax

    movl %eax, wynik(, %esi, 1)
    pushl %eax 

    pushf

    jmp uzupelnienie_esi

uzupelnienie_edi:                   # powrot, jesli druga liczba jest dluzsza
    cmp $0, %edi
    jz koniec_dodawania

    subl $4, %edi

    popf

    movl liczba2(, %edi, 1), %eax
    adcl $0, %eax

    movl %eax, wynik(, %esi, 1)
    pushl %eax 

    pushf

    jmp uzupelnienie_edi

koniec_dodawania:
    cmp $0, %edi                    # sprawdz czy druga liczba jest dluzsza
    jnz uzupelnienie_edi            # jesli tak, kontynuuj dodawanie    
    
    popf
    jnc koniec_programu

    movl $1, %eax
    movl %eax, przeniesienie
    pushl %eax

koniec_programu:
    mov $SYSEXIT, %eax          # wywołanie systemowego wyjścia
    mov $EXIT_SUCCESS, %ebx     # kod powrotu
    int $0x80                   # przerwanie systemowe

# zmienne
.data
    SYSEXIT = 1
    EXIT_SUCCESS = 0

liczba1:
    	.long 0x11000011, 0x10000022, 0x22002211 # .long 0x11000011, 0x10000022, 0x22002211, 0x11002211
        liczba1_len = . - liczba1
liczba2:
    	.long 0xff000000, 0xff001100, 0x22001111, 0x22111100 # .long 0xff000000, 0xff001100, 0x22001111, 0x22111100
        liczba2_len = . - liczba2

.bss
przeniesienie:                      # sprawdzenie wyniku w pamieci za pomoca gdb: x /6x &carry
    .space 4 
wynik:    
    .space liczba1_len
