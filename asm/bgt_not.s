	.file	"bge.c"
	.option pic
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	zero,-20(s0)
	li	a5,10
	sw	a5,-24(s0)
	li	a5,1
	sw	a5,-28(s0)
	sw	zero,-32(s0)
	li	a5,15
	sw	a5,-36(s0)
	li	a5,20
	sw	a5,-40(s0)
	lw	a4,-20(s0)
	lw	a5,-28(s0)
	bgt	a4,a5,.L2
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
	j	.L3
.L2:
	lw	a4,-32(s0)
	lw	a5,-36(s0)
	bne	a4,a5,.L4
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
	j	.L3
.L4:
	lw	a5,-36(s0)
	sw	a5,-32(s0)
.L3:
	li	a5,0
	mv	a0,a5
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
