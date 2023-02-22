; 工具函数库
mycode segment

; void* memcpy( void* dest, const void* src, std::size_t count )
memcpy proc far stdcall uses cx si di es dest:word, src:word, count:word

    mov ax, ds
    mov es, ax

    mov cx, count
    mov si, src
    mov di, dest
    rep movsb

    mov ax, dest
    ret
memcpy endp

; int memcmp( const void* lhs, const void* rhs, std::size_t count )
memcmp proc far stdcall uses cx si di es lhs:word, rhs:word, count:word

    mov ax, ds
    mov es, ax

    mov cx, count
    mov si, lhs
    mov di, rhs
    repz cmpsb

    jz EQUAL
    jl LESS

    mov ax, 1
    ret

LESS: 
    mov ax, -1
    ret
EQUAL:

    mov ax, 0
    ret
memcmp endp

; void * memset ( void * ptr, int value, size_t num );
memset proc far stdcall uses cx di bx es pdest:word, value:word, count:word

    mov ax, ds
    mov es, ax

    mov cx, count
    mov di, pdest
    mov bx, value
    mov al, bl
    rep stosb

    mov ax, pdest
    ret
memset endp

; size_t strlen ( const char * str );
strlen proc far stdcall uses cx si pStr:word
    mov cx, 0
    mov si, pStr
WHILEFN:
    cmp byte ptr [si], '$'
    jz WHILEEND 
    inc si
    inc cx
    jmp WHILEFN
WHILEEND:
    mov ax, cx
    ret
strlen endp


; char * strcpy ( char * destination, const char * source );
strcpy proc far stdcall uses cx si di es destination:word, source:word

    mov ax, ds
    mov es, ax

    invoke strlen, source

    mov cx, ax
    mov si, source
    mov di, destination
    rep movsb
    mov al, '$'
    stosb

    mov ax, destination
    ret
strcpy endp

; int strcmp ( const char * str1, const char * str2 );
strcmp proc far stdcall uses cx si di es str1:word, str2:word

    mov ax, ds
    mov es, ax

    invoke strlen, str1

    mov cx, ax
    mov si, str1
    mov di, str2
    repz cmpsb

    jz EQUAL
    jl LESS

    mov ax, 1
    ret

LESS: 
    mov ax, -1
    ret
EQUAL:
    cmp byte ptr [di], '$'
    jnz LESS
    mov ax, 0
    ret
strcmp endp


; char * strcat ( char * destination, const char * source );
strcat proc far stdcall uses cx si di es destination:word, source:word

    mov ax, ds
    mov es, ax

    invoke strlen, destination

    mov si, source
    mov di, destination
    add di, ax

    invoke strlen, source
    mov cx, ax

    rep movsb
    mov al, '$'
    stosb

    mov ax, destination
    ret
strcat endp

clsfn proc far stdcall

    ; mov ah,06h
    ; mov al,0
    ; mov ch,0
    ; mov cl,0
    ; mov dh,24
    ; mov dl,79
    ; mov bh,07h 
    ; int 10h
    
    mov ah, 00h
    mov al, 03h
    int 10h
    ret
clsfn endp

clear_buf proc far stdcall uses bx pdest:word, count:word
    mov ax, 0
    mov bx, pdest
    inc bx
    mov byte ptr [bx], al
    mov bx, pdest
    add bx, 2
    invoke memset, bx, '$', count
    ret
clear_buf endp

write_file proc far stdcall uses dx cx bx pFileName:word, pBuf:word, count: word
    mov ah, 3ch
    mov dx, pFileName
    mov cx, 00h
    int 21h
    jc FAILED

    mov bx, ax
    mov ah, 40h
    mov cx, count
    mov dx, pBuf
    int 21h
    jc FAILED

    mov dx, ax
    mov ah, 3eh
    int 21h

    mov ax, dx
    ret

FAILED:
    mov ax, 00h
    ret
write_file endp

read_file proc far stdcall uses dx cx bx pFileName:word, pBuf:word, count: word
    mov ah, 3dh
    mov dx, pFileName
    mov al, 00h
    int 21h
    jc FAILED

    mov bx, ax
    mov ah, 3fh
    mov cx, count
    mov dx, pBuf
    int 21h
    jc FAILED

    mov dx, ax
    mov ah, 3eh
    int 21h

    mov ax, dx
    ret

FAILED:
    mov ax, 00h
    ret
read_file endp

to_num proc far stdcall uses si bx cx dx di es numStr:word, count: word
    local @result:word
    mov ax, ds
    mov es, ax

    mov ax, 0
    mov @result, 0
    mov si, count
    .WHILE si > 0
        mov ax, 0ah
        mov bx, 0
        mov di, numStr
        add di, si
        dec di
        mov bl, byte ptr [di]
        sub bx, 30h
        mov cx, count
        sub cx, si
        .IF cx > 0
            .WHILE cx > 1 
                mov di, 0ah
                mul di
                dec cx
            .ENDW
            mul bx
            add @result, ax
        .ELSE
            add @result, bx
        .ENDIF
        dec si
    .ENDW
    mov ax, @result
    ret
to_num endp

; 数字变成字符串(仅支持0~ffffh之间)
num_to_str proc far stdcall uses si bx cx dx num:word, numStr:word
    local @num[5]: byte

    mov si, 0
    mov bx, 0ah
    mov dx, 0
    mov ax, num
DIVV:
    mov dx, 0
    div bx; 余数在dx
    add dl, 30h ; 转位ascii码
    mov @num[si], dl
    inc si
    cmp ax, 0 ; 商0即结束
    jnz DIVV ; 商不为0的时候

    ;invoke memset, numStr, '$', si
    mov cx, numStr
    .WHILE si > 0
        invoke memset, cx, @num[si - 1], 1
        inc cx
        dec si
    .ENDW
    ret
num_to_str endp

; 替换得到输入后的回车符为$
del_dh proc far stdcall uses si cx input:word
    mov si, input
    inc si
    mov cx, input
    add cx, 2
    add cl, [si]
    adc ch, 0
    invoke memset, cx, '$', 1
    ret
del_dh endp

mycode ends
end