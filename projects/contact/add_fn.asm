; 添加相关的函数
include utils.inc
include show_fn.inc
include define.inc

mycode segment

add_contact proc far stdcall uses bx dx cx si input:word, contInfo: word, contNum:word, buff:word, pFile:word
    ; 重置缓存区
    invoke memset, contInfo, 0, size Cont
    mov bx, contInfo
    assume bx:ptr Cont

    ; 输入联络人姓名
INPUTNAME:
    invoke clear_buf, input, 255
    invoke show_add_menu, 0
    mov ah, 0ah
    mov dx, input
    int 21h
    add dx, 2
    invoke del_dh, input

    invoke strlen, dx
    .IF ax < 49
        invoke strcpy, addr [bx].m_sName, dx
    .ELSE
        invoke show_failed
        jmp INPUTNAME
    .ENDIF


    ; 输入联络人电话
INPUTTEL: 
    invoke clear_buf, input, 255
    invoke show_add_menu, 1
    mov ah, 0ah
    mov dx, input
    int 21h
    add dx, 2
    invoke del_dh, input

    invoke strlen, dx
    .IF ax < 12
        invoke strcpy, addr [bx].m_sTel, dx
    .ELSE
        invoke show_failed
        jmp INPUTTEL
    .ENDIF


   
    ; 输入联络人地址
INPUTADDR:
    invoke clear_buf, input, 255
    invoke show_add_menu, 2
    mov ah, 0ah
    mov dx, input
    int 21h
    add dx, 2
    invoke del_dh, input
    
    invoke strlen, dx
    .IF ax < 254
        invoke strcpy, addr [bx].m_sAddr, dx
    .ELSE
        invoke show_failed
        jmp INPUTADDR
    .ENDIF


    ; 计算联络人id
    mov si, contNum
    mov dx, word ptr [si]
    .IF dx == 0
        mov cx, buff
        mov [bx].m_nID, dx
    .ELSEIF dx > 0
        ; 计算当前最后一个联络人的位置
        mov ax, word ptr [si]
        dec ax
        mov dx, size Cont
        mul dx
        mov si, buff
        add si, ax
        ; 以最新的联络人的id往后增加
        mov dx, word ptr [si]
        inc dx
        mov [bx].m_nID, dx
        mov cx, si
        add cx, size Cont
    .ENDIF

    mov si, contNum
    inc word ptr [si]
    invoke memcpy, cx, contInfo, size Cont
    assume bx:nothing
    mov ax, contInfo

    ; 写入文件
    invoke write_file, pFile, contNum, 61442
    ; 读取文件至缓存区
    invoke read_file, pFile, contNum, 61442

    invoke show_success
    invoke show_next
    mov ah, 0ah
    mov dx, input
    int 21h
    invoke show_main_menu
    ret
add_contact endp

mycode ends
end