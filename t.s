.data
.text
main:
    li      $t6,    1
    li      $t7,    1
    add     $t5,    $t6,    $t7
    li      $t6,    1
    add     $t4,    $t5,    $t6
    li      $t5,    1
    add     $t3,    $t4,    $t5
    li      $t4,    2
    add     $t2,    $t3,    $t4
    li      $t3,    3
    add     $t1,    $t2,    $t3
    li      $t2,    4
    add     $t0,    $t1,    $t2
    li      $v0,    1               # system call for print integer
    move    $a0,    $t0             # integer value to print
    syscall 
    li      $v0,    10
    syscall 