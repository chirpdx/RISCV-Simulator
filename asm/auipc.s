	lui t3,1
	lui t4,2
	addi t5,t0,15
	lui t6,1024290
	lui a3,30
	add t6,t5,t4
	sub a1,t5,t4
	sub a2,t4,t5
	auipc a2,1048575
	auipc t3,12345
	auipc t4,0
	jr	ra
