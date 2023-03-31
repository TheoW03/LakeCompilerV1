.data 
 .text 
 main: 

	li $t2,1
	li $t3,2
	mult $t2, $t3 
	mflo $t1 
	li $t5,4
	li $t6,8
	mult $t5, $t6 
	mflo $t4 
	li $t5,10
	add $t3,$t4, $t5 
	li $t5,10
	li $t6,2
	add $t4,$t5, $t6 
	add $t2,$t3, $t4 
	add $t0,$t1, $t2 

	li $v0, 1 
	move $a0,  
	syscall # prints to console

	li $v0, 10 
	syscall # exited program pop into QtSpim and it should work
