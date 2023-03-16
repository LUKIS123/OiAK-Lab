.data

    SYSEXIT = 1
    EXIT_SUCCESS = 0

    SYSWRITE = 4
    STDOUT = 1

.text

    msg: .ascii "Hello! \n"
    msg_len = . - msg

.global _start
_start:

    # wywołanie systemowego zapisu do STDOUT (konsoli)
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg, %ecx
    mov $msg_len, %edx
    int $0x80

    mov $SYSEXIT, %eax          # wywołanie systemowego wyjścia
    mov $EXIT_SUCCESS, %ebx     # kod powrotu
    int $0x80                   # przerwanie systemowe
