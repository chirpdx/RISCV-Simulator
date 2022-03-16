	addi t0,t0,-32
	addi t2,zero,32
	addi t1,t1,32
	addi t1,zero, 32
	addi t2, t2, 2047
	addi t3, t0, -2048
	ori t0, t0, -10
	ori t1, t1, 10
	andi t0, t0, -1
	andi t1, t1, 1
	jr	ra
