	slti t0,t1,32
	slti t0,t1,0
	slti t2,zero,-32
	xori t3,t1,32
	xori t1,zero,-32
	addi t2, t2,2047
	slti t0,t1,1
	sltiu t0,t1,1
	sltiu t3, zero,2000
	sltiu t4,t3,-2040
	sltiu t3,t4,-2040
	xori t1, t1, 10
	xori t1, t0, -1
	jr	ra
