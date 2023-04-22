.text
.global _start
_start:
    # assembler automatcznie startuje w precyzji extended 80 bit, ponizej ustawianie na single - float
    # wiecej na stronie: http://www.ray.masmcode.com/tutorial/fpuchap1.htm
    # bits 9 and 8 determines to what precision the FPU rounds results

    fstcw control
    movw control, %ax

prec1:
    #andw  $0b1111110011111111, %ax      # set precision to single
    #orw    $0b0000110000000000, %ax     # truncate, czyli round towards zero

    #andw    $0b1111000011111111, %ax
    #orw     $0b0000010000000000, %ax   # round down

    andw    $0b1111000011111111, %ax 
    orw     $0b0000100000000000, %ax   # round up

    movw %ax, control

setup:
    fldcw control       # zaladowanie slowa sterjacego do rejestru sterujacego
    call stload

addnum:     # zwykle dodawanie
    faddp %st, %st(1)           # dodaje i wrzuca wynik na stos st (0)

    call stload

subnum:     # generuje +0
    fsubrp %st(0), %st(1)       # liczba1 - liczba2

    flds liczba5
    flds liczba4

mulnum:    # generuje -0
    # fmul %st(1), %st(0)
    fmulp %st, %st(1)           # wariant czysci st(1), wynik na st(0)

    flds liczba5
    flds liczba3


# set precision to double
prec2:
    fstcw control
    movw control, %ax
    orw $0b0000111000000000, %ax     # prescision to double + round towards zero
    movw %ax, control
    fldcw control


divnum1:    # generuje +INF
    fdivp %st, %st(1)           # dzieli liczba2/liczba1 i wrzuca wynik na stos st (0)

    flds liczba5
    flds liczba4

divnum2:    # generuje -INF
    fdivp %st, %st(1)

    flds liczba4

sqrtnum:    # generuje NaN
    fsqrt                       # sqrt z liczba2 (-1) do wygenerowania NaN

    flds liczba3
    flds liczba6

divnum3:    # liczba zdenormalizowana
    fdivp %st, %st(1)

koniec:
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80

stload:
    flds liczba1
    flds liczba2
    ret    

.data
    SYSEXIT = 1
    EXIT_SUCCESS = 0

control: 
    .word 0

liczba1:
    .float 1.2
liczba2:
    .float 1.2
liczba3:
    .float 1.0
liczba4:
    .float -1.0
liczba5:
    .float 0.0        
liczba6:
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
