; 添加相关的函数
include utils.inc
include show_fn.inc
include define.inc

mycode segment

del_contact proc far stdcall uses bx dx cx si di contInfo: word, contNum:word, buff:word, buffEnd:word, pFile:word
    ; 重置缓存区
    invoke memset, contInfo, 0, size Cont
    mov bx, contInfo
    assume bx:ptr Cont

    mov di, buff
    mov cx, buff
    add cx, size Cont
    .WHILE di < buffEnd
        .IF cx == buffEnd
            invoke memset, di, '$', size Cont
            jmp ENDSET
        .ENDIF
        invoke memcpy, di, cx, size Cont
        mov di, cx
        add cx, size Cont
    .ENDW
    
ENDSET:
    mov si, contNum
    dec word ptr [si]

    assume bx:nothing
    mov ax, contInfo

    ; 写入文件
    invoke write_file, pFile, contNum, 61442
    ; 读取文件至缓存区
    invoke read_file, pFile, contNum, 61442
    ret
del_contact endp

mycode ends
end