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

mycode ends
end