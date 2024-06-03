
ESP0 = 4
KERNEL_STACK = 12

.globl switch_to

switch_to:
    pushl %ebp
    movl %esp,%ebp
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl 8(%ebp),%ebx
    cmpl %ebx,current
    je 1f
# exchange PCB
    movl %ebx,%eax
    xchgl %eax,current
# TSS中的内核栈指针的重写
    movl tss,%ecx
    addl $4096,%ebx
    movl %ebx,ESP0(%ecx)
# exchange kernel stack
    movl %esp,KERNEL_STACK(%eax)
# reget ebx
    movl 8(%ebp),%ebx
    movl KERNEL_STACK(%ebx),%esp
# exchange LDT
    movl 12(%ebp),%ecx
    lldt %cx
    movl $0x17,%ecx
    mov %cx,%fs
# hanle clts etc
    cmpl %eax,last_task_used_math
    jne 1f
    clts

1:  popl %eax
    popl %ebx
    popl %ecx
    popl %ebp
    ret