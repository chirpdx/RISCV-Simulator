	lui t1,3
	addi t1,t1,-1366
	lui t2,262144	
	addi t2,t2,-1
	li t4,10
	xori t3,t4,-1 #logical inversion
	xor t3,t1,t2	
	jr ra
	