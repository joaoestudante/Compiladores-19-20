; TEXT
segment	.text
; ALIGN
align	4
; GLOBAL _main, :function
global	_main:function
; LABEL _main
_main:
; ENTER 0
	push	ebp
	mov	ebp, esp
	sub	esp, 0
; INT 8
	push	dword 8
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN printd
extern	printd
; EXTERN println
extern	println
; EXTERN printi
extern	printi
; EXTERN prints
extern	prints
; EXTERN readi
extern	readi
; EXTERN readd
extern	readd
