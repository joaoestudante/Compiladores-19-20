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
; INT 0
	push	dword 0
; JZ _L1
	pop	eax
	cmp	eax, byte 0
	je	near _L1
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L2
_L2:
; SSTRING (value omitted -- see below)
	db	"KO", 0
; TEXT
segment	.text
; ADDR _L2
	push	dword $_L2
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L3
	jmp	dword _L3
; LABEL _L1
_L1:
; INT 1
	push	dword 1
; JZ _L4
	pop	eax
	cmp	eax, byte 0
	je	near _L4
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L5
_L5:
; SSTRING (value omitted -- see below)
	db	"elif", 0
; TEXT
segment	.text
; ADDR _L5
	push	dword $_L5
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L6
	jmp	dword _L6
; LABEL _L4
_L4:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L7
_L7:
; SSTRING (value omitted -- see below)
	db	"OK", 0
; TEXT
segment	.text
; ADDR _L7
	push	dword $_L7
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; LABEL _L6
_L6:
; LABEL _L3
_L3:
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
