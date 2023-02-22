assume ds:mydata
include utils.inc

mystack segment stack
    db 512 dup(0)
mystack ends

mydata segment
    g_src db 64 dup(11h)
    g_dst db 64 dup(0)

    g_lhs db "111111111", '$'
    g_rhs db "112111111", '$'

    g_dst2 db 64 dup(?)
    g_dst3 db 10 dup(?)

    g_src2 db "hello world", '$'
    g_dst4 db "123456789", 0dh ,'$', 11 dup(?)
mydata ends

mycode segment

START:
    mov ax, mydata
    mov ds, ax

    invoke strcat, offset g_dst4, offset g_src2

    mov ah, 09h
    mov dx, offset g_dst4
    int 21h
    
    invoke strcmp, offset g_lhs, offset g_rhs
    invoke strcpy, offset g_dst3, offset g_lhs
    invoke strlen, offset g_lhs

    invoke memcpy, offset g_dst, offset g_src, offset g_dst - offset g_src
    invoke memcmp, offset g_lhs, offset g_rhs, offset g_rhs - offset g_lhs
    invoke memset, offset g_dst2, 11h, 64
    

    mov ax, 4c00h
    int 21h
    
mycode ends

end START