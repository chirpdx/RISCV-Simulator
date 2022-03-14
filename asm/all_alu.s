	
    li	t3,10               #loading postive value
	li	t4,2                #loading postive value
    add t0,t3,t4            #t0 = 12 
    sub t0,t3,t4            #t0 = 8
    sub t0,t4,t3            #t0 = -8 underflow
	sll	t5,t3,t4            #Shift left Logical 10<<2 = 28_h
	slt	t6,t3,t4            #Set less than 10 < 2  = 0
    slt	t6,t4,t3            #Set less than 2 < 10  = 1  
    sltu a0,t3,t4           #set less than unsigned 10 < 2 = 0         
    xor a1,t3,t4            #xor 1010 xor 10 = 1000(8_h)
    srl a2,t3,t4            #Shift right logical 10>>2 = 2_h             
    sra a3,t3,t4            #shift right arthimetic 10>>2 = 2_h
    or  a4,t3,t4            #or 1010 or 10 = 1010(a_h)
    and a5,t3,t4            #and 1010 and 10 = 0010(2_h)
    slti  a6,t4,2           #set less than immediate 2<2 =0?
    slti  a6,t4,1           #set less than immediate 2<1 =0?
    slti  a6,t4,3           #set less than immediate 2<3 =1? 
    ori  a7,t4,8            #ori 10 or 1000 = 1010
    addi a7,t4,2047         #checking overflow
    li t3,-10               #loading negative value
    li t4,-30               #loading negative value
    add t0,t3,t4            #t0 = -40 
    sub t0,t3,t4            #t0 = 20
    sub t0,t4,t3            #t0 = -8
    sll	t5,t3,t4            #Shift left Logical -10 << -30 = FFFF_FFD8
	slt	t6,t3,t4            #Set less than -10 < -30 = 0
    slt	t6,t4,t3            #Set less than -30 < -10 = 1
    sltu a0,t3,t4           #Set less than unsigned -10 <-30 = 1
    xor a1,t3,t4            #xor -10 xor -30 = 14_h
    srl a2,t3,t4            #Shift right probably 3FFF_FFFD
    sra a3,t3,t4            #shift right arthimetic -10>> -30 = FFFF_FFFD
    or  a4,t3,t4            #or -10 or -30 = FFFF_FFF6
    and a5,t3,t4            #and -10 and -30 = FFFF_FFE2
    slti  a6,t4,2           #set less than immediate -30<2 =0
    slti  a6,t4,-30         #set less than immediate -30<-30 =0
    slti  a6,t4,-33         #set less than immediate -30< -33 =1
    ori  a7,t4,-20          #ori 10 or 1000 = 1010
	jr	ra
