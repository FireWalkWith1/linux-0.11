.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry _start
_start:
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#25
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#msg2
	mov ax,cs
	mov es,ax
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

inf_loop:
	jmp inf_loop
	
msg2:
	.byte 13,10
	.ascii "Now we are in SETUP"
	.byte 13,10,13,10

.org 510
boot_flag:
	.word 0xAA55

.text
endtext:
.data
enddata:
.bss
endbss:
