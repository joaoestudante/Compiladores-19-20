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
; INT 0
	push	dword 0
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
	db	"elif1", 0
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
; INT 0
	push	dword 0
; JZ _L7
	pop	eax
	cmp	eax, byte 0
	je	near _L7
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L8
_L8:
; SSTRING (value omitted -- see below)
	db	"elif2", 0
; TEXT
segment	.text
; ADDR _L8
	push	dword $_L8
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L9
	jmp	dword _L9
; LABEL _L7
_L7:
; INT 0
	push	dword 0
; JZ _L10
	pop	eax
	cmp	eax, byte 0
	je	near _L10
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L11
_L11:
; SSTRING (value omitted -- see below)
	db	"elif3", 0
; TEXT
segment	.text
; ADDR _L11
	push	dword $_L11
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L12
	jmp	dword _L12
; LABEL _L10
_L10:
; INT 1
	push	dword 1
; JZ _L13
	pop	eax
	cmp	eax, byte 0
	je	near _L13
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L14
_L14:
; SSTRING (value omitted -- see below)
	db	"elif4", 0
; TEXT
segment	.text
; ADDR _L14
	push	dword $_L14
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; JMP _L15
	jmp	dword _L15
; LABEL _L13
_L13:
; RODATA
segment	.rodata
; ALIGN
align	4
; LABEL _L16
_L16:
; SSTRING (value omitted -- see below)
	db	"OK", 0
; TEXT
segment	.text
; ADDR _L16
	push	dword $_L16
; CALL prints
	call	prints
; TRASH 4
	add	esp, 4
; LABEL _L15
_L15:
; LABEL _L12
_L12:
; LABEL _L9
_L9:
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
