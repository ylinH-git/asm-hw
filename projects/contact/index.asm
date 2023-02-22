assume ds:mydata
include define.inc
include utils.inc
include show_fn.inc
include add_fn.inc
include delete_fn.inc
include modify_fn.inc
include search_fn.inc

mystack segment stack
    db 512 dup(0)
mystack ends

mydata segment
    g_rn db 0dh, 0ah, '$'
    g_print_conf db 323 dup(' ')
    g_contInfo Cont <?>
    g_file db "contact.bin", 0
    g_buf db 255, ?, 255 dup('$')
    g_contNum dw 0
    g_contBuf db 61440 dup('$')
    g_contBufEnd db 0
mydata ends

mycode segment

START:
    mov ax, mydata
    mov ds, ax
    mov es, ax

    ; 读取文件至缓存区
    invoke read_file, offset g_file, offset g_contNum, 61442
    invoke show_main_menu
.WHILE 1
    mov ah, 0ah
    mov dx, offset g_buf
    int 21h

    .IF byte ptr [g_buf + 2] == '5'
        jmp CLOSEAPP
    .ELSEIF byte ptr [g_buf + 2] == '1'
        invoke add_contact, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf, offset g_file
    .ELSEIF byte ptr [g_buf + 2] == '2'
        invoke search_by_id, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf
        mov bx, ax
        .IF bx != 0
            invoke modify_contact, offset g_buf, offset g_contInfo, offset g_contNum, bx, offset g_file
        .ELSE
            invoke show_not_found
            invoke show_next
            mov ah, 0ah
            mov dx, offset g_buf
            int 21h
            invoke show_main_menu
        .ENDIF
       
    .ELSEIF byte ptr [g_buf + 2] == '3'
        invoke show_search_menu
        mov ah, 0ah
        mov dx, offset g_buf
        int 21h
        .IF byte ptr [g_buf + 2] == '1'
            invoke search_by_id, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf
            mov bx, ax
            .IF bx != 0
                invoke memcpy, offset g_contInfo, bx, size Cont
                invoke show_list_title
                invoke show_conf_info, offset g_contInfo, offset g_print_conf, offset g_rn
                invoke show_next
                mov ah, 0ah
                mov dx, offset g_buf
                int 21h
                invoke show_main_menu
            .ELSE
                invoke show_not_found
                invoke show_next
                mov ah, 0ah
                mov dx, offset g_buf
                int 21h
                invoke show_main_menu
            .ENDIF
        .ELSEIF byte ptr [g_buf + 2] == '2'
            invoke search_by_name, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf, offset g_print_conf, offset g_rn
            invoke show_next
            mov ah, 0ah
            mov dx, offset g_buf
            int 21h
            invoke show_main_menu
        .ELSEIF byte ptr [g_buf + 2] == '3'
            invoke search_by_tel, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf, offset g_print_conf, offset g_rn
            invoke show_next
            mov ah, 0ah
            mov dx, offset g_buf
            int 21h
            invoke show_main_menu
        .ELSEIF byte ptr [g_buf + 2] == '4'
            invoke search_by_addr, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf, offset g_print_conf, offset g_rn
            invoke show_next
            mov ah, 0ah
            mov dx, offset g_buf
            int 21h
            invoke show_main_menu          
        .ELSEIF byte ptr [g_buf + 2] == '5'
            invoke show_main_menu
        .ENDIF
    .ELSEIF byte ptr [g_buf + 2] == '4'
            invoke search_by_id, offset g_buf, offset g_contInfo, offset g_contNum, offset g_contBuf
            mov bx, ax
            .IF bx != 0
                invoke del_contact, offset g_contInfo, offset g_contNum, bx, offset g_contBufEnd, offset g_file
                invoke show_next
                mov ah, 0ah
                mov dx, offset g_buf
                int 21h
                invoke show_main_menu
            .ELSE
                invoke show_not_found
                invoke show_next
                mov ah, 0ah
                mov dx, offset g_buf
                int 21h
                invoke show_main_menu
            .ENDIF
    .ENDIF
    invoke clear_buf, offset g_buf, 255
.ENDW
   

CLOSEAPP:
    mov ax, 4c00h
    int 21h
    
mycode ends

end START