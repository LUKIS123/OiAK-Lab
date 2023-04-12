.code32
.text
.global _start
_start:

    clc
    pushf
    movl $liczba1_len, %esi

dodawanie:
    subl $4, %esi

    popf
    movl liczba1(,%esi,1), %eax
    adcl liczba2(,%esi,1), %eax

    pushl %eax
    pushf

    cmp $0, %esi
    jz koniec_dodawania

    jmp dodawanie

koniec_dodawania:
    popf
    jnc koniec_programu
    pushl $0x00000001

koniec_programu:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80

.data

    SYSEXIT = 1
    EXIT_SUCCESS = 0

liczba1:
    	.long 0x10304008, 0x701100FF, 0x45100020, 0x08570030
        liczba1_len = . - liczba1
liczba2:
    	.long 0xF040500C, 0x00220026, 0x321000CB, 0x04520031
        # liczba2_len = . - liczba2
