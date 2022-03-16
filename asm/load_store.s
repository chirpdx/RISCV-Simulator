	lui t0,20234
	addi t2,t0,-102
	sw t2,4(t3)
	lw t4,4(t3)
	sb t2,1(t3)
	lb t5,1(t3)
	lbu a0,1(t3)
	sh t2,2(t3)	
	lh t6,2(t3)
	lhu a1,2(t3)
	lw t4,3(t3) #Non Aligned address for 4 byte
	lh t6,1(t3) #Non Aligned address for 2 byte
	sw t2,3(t3) #Non Aligned address for 4 byte
	sh t2,1(t3) #Non Aligned address for 2 byte
	sb t2,5(t3)
	lb s4,5(t3)
	sw t2,0(t3)
	lw s0,0(t3)
	lw t3,-1(t0) #Non Aligned address for 4 byte
	lw t4,-2(t0) #Non Aligned address for 4 byte
	jr ra
