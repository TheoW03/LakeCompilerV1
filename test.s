.data 
 .text 
 main: 

	li $t5,1
	li $t6,1
	add $t4,$t5, $t6 
	li $t5,3
	add $t3,$t4, $t5 
	li $t4,5
	add $t2,$t3, $t4 
	li $t3,6
	add $t1,$t2, $t3 
	li $t3,8
	li $t4,10
	mult $t3, $t4 
	mflo $t2 
	add $t0,$t1, $t2 

	li $v0, 1 
	move $a0,$t0
	syscall # prints to console

	li $v0, 10 
	syscall # exited program pop into QtSpim and it should work
