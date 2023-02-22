; 添加相关的函数
include utils.inc
include show_fn.inc
include define.inc

mycode segment

modify_contact proc far stdcall uses bx dx cx si di input:word, contInfo: word, contNum:word, buff:word, pFile:word
    ; 重置缓存区
    invoke memcpy, contInfo, buff, size Cont
    mov bx, contInfo
    assume bx:ptr Cont

    .WHILE 1
        invoke clear_buf, input, 255
        invoke show_modify_menu
        mov ah, 0ah
        mov si, input
        int 21h
        add si, 2
        .IF byte ptr [si] == '1'
INPUTNAME:
            invoke show_enter_name
            mov ah, 0ah
            mov dx, input
            int 21h
            invoke del_dh, input
            mov di, input
            add di, 2

            invoke strlen, di
            .IF ax < 49
                invoke memset, addr [bx].m_sName, 0, 50 
                invoke strcpy, addr [bx].m_sName, di
            .ELSE
                invoke show_failed
                jmp INPUTNAME
            .ENDIF

        .ELSEIF byte ptr [si] == '2'
INPUTTEL:
            invoke show_enter_tel
            mov ah, 0ah
            mov dx, input
            int 21h
            invoke del_dh, input
            mov di, input
            add di, 2
            invoke strlen, di
            .IF ax < 12
                invoke memset, addr [bx].m_sTel, 0, 13 
                invoke strcpy, addr [bx].m_sTel, di
            .ELSE
                invoke show_failed
                jmp INPUTTEL
            .ENDIF
        .ELSEIF byte ptr [si] == '3'
INPUTADDR:
            invoke show_enter_addr
            mov ah, 0ah
            mov dx, input
            int 21h
            invoke del_dh, input
            mov di, input
            add di, 2
            invoke strlen, di
            .IF ax < 12
                invoke memset, addr [bx].m_sAddr, 0, 255
                invoke strcpy, addr [bx].m_sAddr, di
            .ELSE
                invoke show_failed
                jmp INPUTADDR
            .ENDIF
        .ELSEIF byte ptr [si] == '4'    
            jmp ENDMODIFY
        .ENDIF

        invoke memcpy, buff, contInfo, size Cont
        ; 写入文件
        invoke write_file, pFile, contNum, 61442
        ; 读取文件至缓存区
        invoke read_file, pFile, contNum, 61442
    .ENDW

ENDMODIFY:
    assume bx:nothing
    mov ax, contInfo
    invoke show_main_menu
    ret
modify_contact endp

mycode ends
end