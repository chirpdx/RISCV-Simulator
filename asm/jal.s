		addi t0,t0,32
		addi a0,a0,-1
		addi a5,a5,1
		jal t0,jump
		addi a5,a5,0
jump:	add a0,a0,a5
		jal t3,end
end:	addi sp,sp,32
		jr ra
		