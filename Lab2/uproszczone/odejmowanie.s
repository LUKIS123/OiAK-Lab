.code32
.text
.global _start
_start:

    clc
    pushf

    movl $liczba1_len, %esi

odejmowanie:
    subl $4, %esi

    popf
    movl liczba1(,%esi,1), %eax

    jnc dalej_odejmowanie
    subl $1, %eax

dalej_odejmowanie:
    subl liczba2(,%esi,1), %eax
    pushl %eax
    pushf

    cmp $0, %esi
    jz koniec_odejmowania

    jmp odejmowanie

koniec_odejmowania:
    popf
    jnc koniec_programu
    pushl $0xFFFFFFFF

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
