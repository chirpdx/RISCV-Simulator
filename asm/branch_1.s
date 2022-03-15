		lui t4,4
		addi t4,t4,235
label0:	bge a2,a3,label1
		sub a1,t5,t4
		sub a0,t4,t5

label1:	lui t5,4
		lui t6,7
		bne t5,t6,label2
		sub a1,t5,t4
		sub a0,t4,t5

label2: sub a1,t5,t4
		sub a0,t4,t5
		jal x0,ra,0
		