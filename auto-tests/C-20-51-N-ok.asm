; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L1
_L1:
; SSTRING (value omitted -- see below)
	db	"ole", 0
; TEXT
segment	.text
; ADDR _L1
	push	dword $_L1
; TRASH 4
	add	esp, 4
; DATA
segment	.data
; ALIGN
align	4
; LABEL x
x:
; SADDR _L1
	dd	_L1
; TEXT
segment	.text
; ALIGN
align	4
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
; ADDR x
	push	dword $x
; LDINT
	pop	eax
	push	dword [eax]
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; INT 0
	push	dword 0
; STFVAL32
	pop	eax
; LEAVE
	leave
; RET
	ret
; EXTERN prints
extern	prints
