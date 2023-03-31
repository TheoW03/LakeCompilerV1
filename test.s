.data 
 .text 
 main: 

	li $t2,10
	li $t3,2
	div $t1,$t2, $t3 
	mfhi $t1
	li $t4,10
	li $t5,100
	mult $t4, $t5 
	mflo $t3 
	li $t5,7
	li $t6,2
	div $t4,$t5, $t6 
	mfhi $t4
	mult $t3, $t4 
	mflo $t2 
	add $t0,$t1, $t2 

	li $v0, 1 
	move $a0,$t0
	syscall # prints to console

	li $v0, 10 
	syscall # exited program pop into QtSpim and it should work
