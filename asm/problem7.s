	.file	"problem7.c"
	.option pic
	.text
	.align	2
	.globl	branch
	.type	branch, @function
branch:
	addi	sp,sp,-32
	sw	s0,28(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,1
	beq	a4,a5,.L2
	li	a5,1
	j	.L3
.L2:
	li	a5,0
.L3:
	mv	a0,a5
	lw	s0,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	branch, .-branch
	.ident	"GCC: (GNU) 9.2.0"
	.section	.note.GNU-stack,"",@progbits
