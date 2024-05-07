.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

INITSEG=0X9000

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

read_param:
	mov as,#INITSEG
	mov ds,ax
	mov ah,#0x03
	xor bh,bh
	int 0x10
	mov [0],dx
	mov ah,#0x88

	mov ah,#0x88,
	int 0x15
	mov [2],ax

	! 从 0x41 处拷贝 16 个字节（磁盘参数表）
	mov	ax,#0x0000
	mov	ds,ax
	lds si,[4*0x41]
	mov ax,#INITSEG
	mov es,ax
	mov di,#0x0004
	mov cx,#0x10
	! 重复16次
	rep
	movsb
	
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
