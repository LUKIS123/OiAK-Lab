.text
.global _start
_start:
    # assembler automatcznie startuje w podwojnjej precyzji double, ponizej ustawianie na single - float
    # wiecej na stronie: http://www.ray.masmcode.com/tutorial/fpuchap1.htm
    # bits 9 and 8 determines to what precision the FPU rounds results

    fstcw control
    movw control, %ax

and:
    andw  $0b1111110011111111, %ax # truncate, czyli do 0
    # andw    $0b1111000011111111, %ax 
    # orw     $0b0000010000000000, %ax # round down
    movw %ax, control

cl:
    fldcw control       # zaladowanie slowa sterjacego do rejestru sterujacego

cs
    fstcw control       # dczytane do bufora w pamieci rozkazem fstcw zeby mod sprawdzic

set:

    flds liczba1
    flds liczba2

calc:

    fmul %st(1), %st(0)
    # fmulp %st, %st(1) # wariant czysci st(1), wynik na st(0)

    # fdivp # dzieli i wrzuca wynik na stos st (0)

    # faddp %st, %st(1)           # dodaje i wrzuca wynik na stos st (0)

    # fsubrp %st(0), %st(1)       # liczba1 - liczba2

koniec:
    mov $SYSEXIT, %eax          # wywołanie systemowego wyjścia
    mov $EXIT_SUCCESS, %ebx     # kod powrotu
    int $0x80                   # przerwanie systemowe

.data
    SYSEXIT = 1
    EXIT_SUCCESS = 0

control: 
    .word 0

liczba1:
    .float 1.2
    
liczba2:
    .float 1.6

# komendy:
# p $st1
# p/t $st1
# p/t (short) control -> do sprawdzenia ustawien precyzji
# p/t $fstat -> do sprawdzenia flag
# info all-registers 