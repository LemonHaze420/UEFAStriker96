	opt	c+,m+
	;include	gtereg.h


	public on

	section .text
	


	cnop	0,4
	
	RSRESET


******************************************************************
* ot de-nuller							 *
******************************************************************
compact_otr:	
	sll 	a1,2			* 4 for bytes
	addu 	a0,a1			get ot end
	lw 	t0,-4(a0)

*****************************************************************
* creat mask in reg for zippy					*
*****************************************************************
	lw 	t1,-8(a0)
	li 	t2,$00FFFFFF

*****************************************************************
* protend its addr is low					*
*****************************************************************
	and 	a0,t2
	subiu 	a0,4
	move 	a2,a0

*****************************************************************
* setup last slot if needbe					*
*****************************************************************
	subu 	a1,a0,a1

*****************************************************************
* a1 is ot end address						*
*****************************************************************
	subiu 	a0,4

@p1:
	beq 	a0,a1,@donep2
	lw 	t2,-4(a0)

@p1_loop:
	beq 	t0,a0,@p2
	addiu 	a0,-4
	sw 	t0,(a2)

@Used2:
	beq 	a0,a1,@doneUsed
	lw 	t0,-4(a0)
	bne 	t1,a0,@Used3
	addiu 	a0,-4
	addiu 	a2,a0,8

@p3:
	beq 	a0,a1,@donep1
	lw 	t1,-4(a0)
	beq 	t2,a0,@p1
	addiu 	a0,-4
	sw 	t2,(a2)

@Used1:
	beq 	a0,a1,@doneUsed
	lw 	t2,-4(a0)
	bne 	t0,a0,@Used2
	addiu 	a0,-4
	addiu 	a2,a0,8

@p2:
	beq 	a0,a1,@donep3
	lw 	t0,-4(a0)
	beq 	t1,a0,@p3
	addiu 	a0,-4
	sw 	t1,(a2)

@Used3:
	beq 	a0,a1,@doneUsed
	lw 	t1,-4(a0)
	bne 	t2,a0,@Used1
	addiu 	a0,-4
	addiu 	a2,a0,8

@p1_loopStart:
	bne 	a0,a1,@p1_loop

*****************************************************************
* loop if NOT at end of OT					*
*****************************************************************
	lw 	t2,-4(a0)

@donep2:
	sw 	t0,(a2)
	jr 	ra
	nop

@donep1:
	sw 	t2,(a2)
	jr 	ra
	nop

@donep3:
	sw 	t1,(a2)

@doneUsed:
	jr 	ra
	nop


