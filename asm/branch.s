		addi t0,t0,10
loop:	beq t0,t1,end
		addi t0,t0,-10
		jal t2,loop
end:	li ra,0
		jr ra
		