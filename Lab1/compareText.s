.data

    SYSEXIT = 1
    EXIT_SUCCESS = 0

    SYSWRITE = 4
    STDOUT = 1

    SYSREAD = 3
    STDIN = 0

    buf: .ascii "     "
    buf_len = . - buf

.text

    msg: .ascii "Write text to compare (5): \n"
    msg_len = . - msg

    msg_same: .ascii "The text is the SAME!\n"
    msg_same_len = . - msg_same

    msg_diff: .ascii "The text is DIFFERENT!\n"
    msg_diff_len = . - msg_diff

    # ciag znakow zapisany w zmiennej w celu porowania
    text_to_compare: .ascii "napis"
    text_to_compare_len = . - text_to_compare

.global _start

_start:

    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg, %ecx
    mov $msg_len, %edx
    int $0x80

    mov $SYSREAD, %eax
    mov $STDIN, %ebx
    mov $buf, %ecx
    mov $buf_len, %edx
    int $0x80

    # ustawienie licznika petli
    # rejestr EDI posluzy takze do "wyciagania" pojedynczych znakow z ciagu
    mov $0, %edi

compare_loop:
    movb buf(,%edi,1), %al
    movb text_to_compare(,%edi,1), %bl 

    cmpb %bl, %al
    jne different

    incl %edi
    cmpl $text_to_compare_len, %edi
    # jump if below => petla
    jb compare_loop


    # jesli petla przejdzie pomyslnie przez wszystkie znaki, wypisz komunikat
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg_same, %ecx
    mov $msg_same_len, %edx
    int $0x80

    # skok do wywołania systemowego wyjścia
    jmp program_exit

different:
    # jesli znaki sie nie zgadzaja, wypisz komunikat
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg_diff, %ecx
    mov $msg_diff_len, %edx
    int $0x80

program_exit:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80

    # ograniczenie jakie naklada wykorzystanie rejestru akumulatora:
    # nie mozna go uzywac do przechowywania calego ciagu znakow
    # rejestr akumulatora AL odnosi sie do "lowest byte" rejestru EAX
    # oznacza to ze moze on przechowywac tylko 1 bajt wartosci (8bit)
    # pojedynczy znak w ASCII ma takze 1 bajt dlugosci, wiec nalzey porownywac kazdy znak osobno
    # co dzieje sie w petli compare_loop
