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

    li      $v0,    1
    move    $a0,    $t0
    syscall                         # prints to console

    li      $v0,    10
    syscall                         # exited program pop into QtSpim and it should work
