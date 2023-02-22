; 添加相关的函数
include utils.inc
include show_fn.inc
include define.inc

mycode segment


show_conf_info proc far stdcall uses dx bx si contInfo:word, printBuf:word, printRn:word
    mov bx, contInfo
    assume bx:ptr Cont
    invoke memset, printBuf, ' ', 143h

    invoke num_to_str, [bx].m_nID, printBuf

    invoke strlen, addr [bx].m_sName
    mov si, ax
    mov dx, printBuf
    add dx, 6
    invoke memcpy, dx, addr [bx].m_sName, si
    invoke strlen, addr [bx].m_sTel
    mov si, ax
    mov dx, printBuf
    add dx, 25
    invoke memcpy, dx, addr [bx].m_sTel, si
    invoke strlen, addr [bx].m_sAddr
    mov si, ax
    mov dx, printBuf
    add dx, 38
    invoke memcpy, dx, addr [bx].m_sAddr, si
    mov dx, printBuf
    add si, 69
    add dx, si
    invoke memcpy, dx, printRn, 3

    mov ah, 09h
    mov dx, printBuf
    int 21h
    ret
show_conf_info endp

; 按id精确搜索并输出
search_by_id proc far stdcall uses bx dx cx si di input:word, contInfo: word, contNum:word, buff:word
    invoke clear_buf, input, 255
    mov bx, contInfo
    assume bx:ptr Cont


    invoke show_enter_id
    mov ah, 0ah
    mov dx, input
    int 21h

    mov si, contNum
    mov cx, 0 
    .WHILE cx < word ptr [si]
        mov di, buff
        mov ax, size Cont
        mul cx
        add di, ax
        invoke memcpy, bx, di, size Cont

        mov dx, di
        mov si, input
        mov di, input
        add si, 2
        inc di
        invoke to_num, si, byte ptr [di]
        mov si, contNum
        mov di, dx
        .IF [bx].m_nID == ax
            assume bx:nothing
            mov ax, di
            ret
        .ENDIF
        inc cx
    .ENDW

    assume bx:nothing
    mov ax, 00h
    ret
search_by_id endp

; 按名称模糊搜索并输出
search_by_name proc far stdcall uses bx dx cx si di input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
    local @temp: word
    local @nameLen: word
    local @namePos: word
    local @cmpStrLen: word
    invoke clear_buf, input, 255
    mov bx, contInfo
    assume bx:ptr Cont
    
    invoke show_enter_name
    mov ah, 0ah
    mov dx, input
    int 21h

    ; 复制输入的字符串
    add dx, 2
    invoke del_dh, input

    mov si, contNum
    mov cx, 0 
    invoke show_list_title
    .WHILE cx < word ptr [si]
        mov di, buff
        mov ax, size Cont
        mul cx
        add di, ax
        invoke memcpy, bx, di, size Cont
        
        mov dx, input
        add dx, 2
        invoke strlen, dx
        mov @cmpStrLen, ax

        ; 循环当前联络人信息
        invoke strlen, addr [bx].m_sName
        mov @nameLen, ax
        lea dx, [bx].m_sName
        mov @namePos, dx
        mov ax, @nameLen
        cmp ax, @cmpStrLen
        jb NEXTCONT

        mov ax, @nameLen
        sub ax, @cmpStrLen
        inc ax
        mov @nameLen, ax
        .WHILE @nameLen != 0
            mov dx, input
            add dx, 2
            invoke memcmp, @namePos, dx, 1
            .IF ax == 0
                mov @temp, cx
                mov cx, 0
                .WHILE cx < @cmpStrLen
                    mov di, @namePos
                    mov si, input
                    add si, 2
                    add si, cx
                    add di, cx
                    mov ax, 0
                    mov al, [si]
                    cmp byte ptr [di], al
                    jz IFEQUAL
                    mov cx, @temp
                    .IF @nameLen >= 1
                        jmp NEXTCHAR
                    .ELSE
                        jmp NEXTCONT
                    .ENDIF
            IFEQUAL:
                    inc cx
                .ENDW
                mov cx, @temp
                jmp SHOWCONF
            .ENDIF
NEXTCHAR:
            dec @nameLen
            inc @namePos
        .ENDW
        jmp NEXTCONT
SHOWCONF:
        invoke show_conf_info, contInfo, printBuf, rnBuf
NEXTCONT: 
        mov si, contNum
        inc cx
    .ENDW

    assume bx:nothing
    ret
search_by_name endp

; 按地址模糊搜索并输出
search_by_addr proc far stdcall uses bx dx cx si input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
    local @temp: word
    local @addrLen: word
    local @addrPos: word
    local @cmpStrLen: word
    invoke clear_buf, input, 255
    mov bx, contInfo
    assume bx:ptr Cont
    
    invoke show_enter_addr
    mov ah, 0ah
    mov dx, input
    int 21h

    ; 复制输入的字符串
    add dx, 2
    invoke del_dh, input

    mov si, contNum
    mov cx, 0 
    invoke show_list_title
    .WHILE cx < word ptr [si]
        mov di, buff
        mov ax, size Cont
        mul cx
        add di, ax
        invoke memcpy, bx, di, size Cont
        
        mov dx, input
        add dx, 2
        invoke strlen, dx
        mov @cmpStrLen, ax
        
        ; 循环当前联络人信息
        invoke strlen, addr [bx].m_sAddr
        mov @addrLen, ax
        lea dx, [bx].m_sAddr
        mov @addrPos, dx
        mov ax, @addrLen
        cmp ax, @cmpStrLen
        jb NEXTCONT

        mov ax, @addrLen
        sub ax, @cmpStrLen
        inc ax
        mov @addrLen, ax
        .WHILE @addrLen != 0
            mov dx, input
            add dx, 2
            invoke memcmp, @addrPos, dx, 1
            .IF ax == 0
                mov @temp, cx
                mov cx, 0
                .WHILE cx < @cmpStrLen
                    mov di, @addrPos
                    mov si, input
                    add si, 2
                    add si, cx
                    add di, cx
                    mov ax, 0
                    mov al, [si]
                    cmp byte ptr [di], al
                    jz IFEQUAL
                    mov cx, @temp
                    .IF @addrLen >= 1
                        jmp NEXTCHAR
                    .ELSE
                        jmp NEXTCONT
                    .ENDIF
            IFEQUAL:
                    inc cx
                .ENDW
                mov cx, @temp
                jmp SHOWCONF
            .ENDIF
NEXTCHAR:
            dec @addrLen
            inc @addrPos
        .ENDW
        jmp NEXTCONT
SHOWCONF:
        invoke show_conf_info, contInfo, printBuf, rnBuf
NEXTCONT: 
        mov si, contNum
        inc cx
    .ENDW

    assume bx:nothing
    ret
search_by_addr endp

; 按电话模糊搜索并输出
search_by_tel proc far stdcall uses bx dx cx si input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
    local @temp: word
    local @telLen: word
    local @telPos: word
    local @cmpStrLen: word
    invoke clear_buf, input, 255
    mov bx, contInfo
    assume bx:ptr Cont
    
    invoke show_enter_tel
    mov ah, 0ah
    mov dx, input
    int 21h

    ; 复制输入的字符串
    add dx, 2
    invoke del_dh, input

    mov si, contNum
    mov cx, 0 
    invoke show_list_title
    .WHILE cx < word ptr [si]
        mov di, buff
        mov ax, size Cont
        mul cx
        add di, ax
        invoke memcpy, bx, di, size Cont
        
        mov dx, input
        add dx, 2
        invoke strlen, dx
        mov @cmpStrLen, ax
        
        ; 循环当前联络人信息
        invoke strlen, addr [bx].m_sTel
        mov @telLen, ax
        lea dx, [bx].m_sTel
        mov @telPos, dx
        mov ax, @telLen
        cmp ax, @cmpStrLen
        jb NEXTCONT

        mov ax, @telLen
        sub ax, @cmpStrLen
        inc ax
        mov @telLen, ax
        .WHILE @telLen != 0
            mov dx, input
            add dx, 2
            invoke memcmp, @telPos, dx, 1
            .IF ax == 0
                mov @temp, cx
                mov cx, 0
                .WHILE cx < @cmpStrLen
                    mov di, @telPos
                    mov si, input
                    add si, 2
                    add si, cx
                    add di, cx
                    mov ax, 0
                    mov al, [si]
                    cmp byte ptr [di], al
                    jz IFEQUAL
                    mov cx, @temp
                    .IF @telLen >= 1
                        jmp NEXTCHAR
                    .ELSE
                        jmp NEXTCONT
                    .ENDIF
            IFEQUAL:
                    inc cx
                .ENDW
                mov cx, @temp
                jmp SHOWCONF
            .ENDIF
NEXTCHAR:
            dec @telLen
            inc @telPos
        .ENDW
        jmp NEXTCONT
SHOWCONF:
        invoke show_conf_info, contInfo, printBuf, rnBuf
NEXTCONT: 
        mov si, contNum
        inc cx
    .ENDW

    assume bx:nothing
    ret
search_by_tel endp

mycode ends
end