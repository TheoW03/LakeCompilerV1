.data
.text
main:

    li      $t2,    1
    li      $t3,    2
    mult    $t2,    $t3
    mflo    $t1
    li      $t3,    4
    li      $t4,    8
    mult    $t3,    $t4
    mflo    $t2
    add     $t0,    $t1,    $t2
    li      $v0,    1               # system call for print integer
    move    $a0,    $t0             # integer value to print
    syscall 
    li      $v0,    10
    syscall                         # exited program pop into QtSpim and it should work
