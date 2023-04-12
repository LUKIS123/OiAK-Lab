.code32
.text
.global _start
_start:
    clc
    pushf
    movl $liczba1_len, %esi
    movl $liczba2_len, %edi

odejmowanie:
    subl $4, %esi
    subl $4, %edi

    popf
    movl liczba1(, %esi, 1), %eax

    jnc dalej_odejmowanie
    subl $1, %eax

dalej_odejmowanie:
    subl liczba2(, %edi, 1), %eax

    movl %eax, wynik(, %esi, 1)     # zapisywanie wyniku dodawania do naszej tablicy wynikow
    pushl %eax                      # zapisywanie wyniku dodawania na stosie

    pushf

    cmp $0, %esi
    jz koniec_odejmowania

    cmp $0, %edi
    jz uzupelnienie_esi

    jmp odejmowanie

uzupelnienie_esi:                   # jesli pierwsza liczba jest dluzsza
    cmp $0, %esi
    jz koniec_odejmowania

    subl $4, %esi

    popf
    movl liczba1(, %esi, 1), %eax
    
    jnc dalej_uzupelnienie_esi
    subl $1, %eax

dalej_uzupelnienie_esi:
    movl %eax, wynik(, %esi, 1)
    pushl %eax 

    pushf

    jmp uzupelnienie_esi

uzupelnienie_edi:
    cmp $0, %edi
    jz koniec_odejmowania

    subl $4, %edi

    popf
    movl liczba2(, %edi, 1), %eax

    jnc dalej_uzupelnienie_edi
    adcl $1, %eax
dalej_uzupelnienie_edi:
    negl %eax
    movl %eax, wynik(, %esi, 1)
    pushl %eax

    pushf

    jmp uzupelnienie_edi


koniec_odejmowania:
    cmp $0, %edi
    jnz uzupelnienie_edi

    popf
    jnc koniec_programu

    movl $0xffffffff, %eax
    movl %eax, przeniesienie
    pushl %eax

koniec_programu:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80

# zmienne
.data
    SYSEXIT = 1
    EXIT_SUCCESS = 0

liczba1:
    	.long 0x11000011, 0x10000022, 0x22002211, 0x11002211 # 0x11000011, 0x10000022, 0x22002211, 0x11002211
        liczba1_len = . - liczba1
liczba2:
    	.long 0xff000000, 0xff001100, 0x22001111, 0x22111100 #.long 0xff000000, 0xff001100, 0x22001111, 0x22111100
        liczba2_len = . - liczba2

.bss
przeniesienie:                      # sprawdzenie wyniku w pamieci za pomoca gdb: x /6x &carry
    .space 4 
wynik:    
    .space liczba1_len
