	addi t0,zero,-32
	addi t1,zero,32
	addi t2,t0,32
	addi t2, t2, 2047
	addi t3, t0, -2048
	ori t0, t3, -2048
	ori t1, t1, 10
	andi t2, t2, -1
	andi t3, t0, 2047
	jr	ra
	