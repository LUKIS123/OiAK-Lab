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

    msg: .ascii "Write text to compare(5): \n"
    msg_len = . - msg

    msg_same: .ascii "The text is the SAME!\n"
    msg_same_len = . - msg_same

    msg_diff: .ascii "The text is DIFFERENT!\n"
    msg_diff_len = . - msg_diff

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

    # setting the loop counter => i=0
    mov $0, %edi
compare_loop:
    movb  text_to_compare(,%edi,1), %bl
    movb  buf(,%edi,1), %al

    cmp %bl, %al
    jne different

    incl %edi
    cmp $text_to_compare_len, %edi
    jb compare_loop

    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg_same, %ecx
    mov $msg_same_len, %edx
    int $0x80

    jmp program_exit

different:
    mov $SYSWRITE, %eax
    mov $STDOUT, %ebx
    mov $msg_diff, %ecx
    mov $msg_diff_len, %edx
    int $0x80

program_exit:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80
