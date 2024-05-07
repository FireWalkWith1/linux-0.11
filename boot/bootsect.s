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
	
	mov	cx,#36
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#msg1
	mov ax,#0x07c0
	mov es,ax
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

msg1:
	.byte 13,10
	.ascii "Hello OS world, my name is JDW"
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
