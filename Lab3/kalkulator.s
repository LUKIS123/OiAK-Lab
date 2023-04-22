.text
.global _start
_start:
    # assembler automatcznie startuje w podwojnjej precyzji double, ponizej ustawianie na single - float
    # wiecej na stronie: http://www.ray.masmcode.com/tutorial/fpuchap1.htm
    # bits 9 and 8 determines to what precision the FPU rounds results

    fstcw control
    movw control, %ax

prec:
    #andw  $0b1111110011111111, %ax      # set precision to single
    #orw    $0b0000110000000000, %ax     # truncate, czyli round towards zero
    
    #andw    $0b1111000011111111, %ax
    #orw     $0b0000010000000000, %ax   # round down

    andw    $0b1111000011111111, %ax 
    orw     $0b0000100000000000, %ax   # round up

    movw %ax, control

cl:
    fldcw control       # zaladowanie slowa sterjacego do rejestru sterujacego

cs
    fstcw control       # odczytane do bufora w pamieci rozkazem fstcw zeby sprawdzic control word

stload:

    flds liczba1
    flds liczba2

calc:
    # fmul %st(1), %st(0)

    # fmulp %st, %st(1)           # wariant czysci st(1), wynik na st(0)

    fdivp %st, %st(1)           # dzieli liczba2/liczba1 i wrzuca wynik na stos st (0)

    # faddp %st, %st(1)           # dodaje i wrzuca wynik na stos st (0)

    # fsubrp %st(0), %st(1)       # liczba1 - liczba2

    # fsqrt                       # sqrt z liczba2 do wygenerowania NaN

koniec:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80

.data
    SYSEXIT = 1
    EXIT_SUCCESS = 0

control: 
    .word 0

liczba1:
    .float 1.0
    
liczba2:
    .float 0.00000000000000000000000000000000000001 

# komendy:
# p $st1
# p/t $st1
# p/t (short) control -> do sprawdzenia ustawien precyzji
# p/t $fstat -> do sprawdzenia flag
# info all-registers

# wyjatki:
# +INF 1.0/0.0
# -INF -1.0/0.0
# NaN 0.0/0.0 albo sqrt -1.0
# Zdenormalizowana 0.00000000000000000000000000000000000001 i patrzymy do fstat p/t $fstat bit [1] ustawiony na 1 oznacza zdenormalizowana
# +0 odejmujemy dowolne takie same wartosci albo 1 * 0
# -0 mnozymy -1 * 0
