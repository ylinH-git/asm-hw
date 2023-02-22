assume ds:mydata
include utils.inc

mystack segment stack
    db 512 dup(0)
mystack ends

mydata segment
    g_num db 1
    g_numStr db 20 dup(?)
    g_numStr2 db "65535"
mydata ends

mycode segment

START:
    mov ax, mydata
    mov ds, ax
    mov es, ax
   
    invoke to_num, offset g_numStr2, 5
    invoke num_to_str, 12312, offset g_numStr
    

    mov ax, 4c00h
    int 21h
    
mycode ends

end START