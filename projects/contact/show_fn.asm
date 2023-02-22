; 展示相关的函数
assume ds:mydata
include utils.inc
include define.inc

mydata segment
    g_mainmenu1 db "1. Add contact information", 0dh, 0ah, '$'
    g_mainmenu2 db "2. Modify contact information", 0dh, 0ah, '$'
    g_mainmenu3 db "3. Search contact information", 0dh, 0ah, '$'
    g_mainmenu4 db "4. Delete contact information", 0dh, 0ah, '$'
    g_mainmenu5 db "5. Close", 0dh, 0ah, '$'
    g_addmenu1 db  0dh, 0ah, "Please enter contact name(only 49 chars):", 0dh, 0ah, '$'
    g_addmenu2 db 0dh, 0ah, "Please enter contact tel(only 12 chars):", 0dh, 0ah, '$'
    g_addmenu3 db 0dh, 0ah, "Please enter contact address(only 254 chars):", 0dh, 0ah, '$'

    g_searchmenu1 db "1. Search contact by Id", 0dh, 0ah, '$'
    g_searchmenu2 db "2. Search contact by name", 0dh, 0ah, '$'
    g_searchmenu3 db "3. Search contact by tel", 0dh, 0ah, '$'
    g_searchmenu4 db "4. Search contact by address", 0dh, 0ah, '$'
    g_searchmenu5 db "5. Close", 0dh, 0ah, '$'
    g_list_title db "id    name               tel          addr", 0dh, 0ah, '$'

    g_modifymenu1 db "1. Modify name", 0dh, 0ah, '$'
    g_modifymenu2 db "2. Modify tel", 0dh, 0ah, '$'
    g_modifymenu3 db "3. Modify address", 0dh, 0ah, '$'
    g_modifymenu4 db "4. Close", 0dh, 0ah, '$'

    g_enter_id db 0dh, 0ah, "Please enter Id: ", 0dh, 0ah, '$'
    g_enter_name db 0dh, 0ah, "Please enter name: ", 0dh, 0ah, '$'
    g_enter_addr db  0dh, 0ah, "Please enter address: ", 0dh, 0ah, '$'
    g_enter_tel db  0dh, 0ah, "Please enter tel: ", 0dh, 0ah, '$'
    g_enter db  0dh, 0ah, "Please enter code no.: ", 0dh, 0ah, '$'
    g_next db 0dh, 0ah, "Please enter key Enter to next...", 0dh, 0ah, '$'
    g_success db 0dh, 0ah, "Success!", 0dh, 0ah, '$'
    g_failed db 0dh, 0ah, "failed, please enter again!", 0dh, 0ah, '$'
    g_not_found db 0dh, 0ah, "Not Found", 0dh, 0ah, '$'
mydata ends

mycode segment

show_list_title proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    invoke clsfn
    mov ah, 09h
    mov dx, offset g_list_title
    int 21h
    ret
show_list_title endp

show_next proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_next
    int 21h
    ret
show_next endp

show_main_menu proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    invoke clsfn
    mov ah, 09h
    mov dx, offset g_mainmenu1
    int 21h

    mov ah, 09h
    mov dx, offset g_mainmenu2
    int 21h

    mov ah, 09h
    mov dx, offset g_mainmenu3
    int 21h

    mov ah, 09h
    mov dx, offset g_mainmenu4
    int 21h

    mov ah, 09h
    mov dx, offset g_mainmenu5
    int 21h

    mov ah, 09h
    mov dx, offset g_enter
    int 21h
    ret
show_main_menu endp

show_modify_menu proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    invoke clsfn
    mov ah, 09h
    mov dx, offset g_modifymenu1
    int 21h

    mov ah, 09h
    mov dx, offset g_modifymenu2
    int 21h

    mov ah, 09h
    mov dx, offset g_modifymenu3
    int 21h

    mov ah, 09h
    mov dx, offset g_modifymenu4
    int 21h

    mov ah, 09h
    mov dx, offset g_enter
    int 21h
    ret
show_modify_menu endp

show_add_menu proc far stdcall uses ds dx addType:word
    mov ax, mydata
    mov ds, ax

    .IF addType == 00h
        mov ah, 09h
        mov dx, offset g_addmenu1
        int 21h
    .ELSEIF  addType == 01h
        mov ah, 09h
        mov dx, offset g_addmenu2
        int 21h
    .ELSEIF  addType == 02h
        mov ah, 09h
        mov dx, offset g_addmenu3
        int 21h
    .ENDIF
    ret
show_add_menu endp

show_search_menu proc far stdcall uses ds dx
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_searchmenu1
    int 21h

    mov ah, 09h
    mov dx, offset g_searchmenu2
    int 21h

    mov ah, 09h
    mov dx, offset g_searchmenu3
    int 21h

    mov ah, 09h
    mov dx, offset g_searchmenu4
    int 21h

    
    mov ah, 09h
    mov dx, offset g_searchmenu5
    int 21h

    mov ah, 09h
    mov dx, offset g_enter
    int 21h
    ret
show_search_menu endp

show_enter_id proc far stdcall uses ds dx
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_enter_id
    int 21h

    ret
show_enter_id endp

show_enter_name proc far stdcall uses ds dx
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_enter_name
    int 21h

    ret
show_enter_name endp

show_enter_addr proc far stdcall uses ds dx
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_enter_addr
    int 21h

    ret
show_enter_addr endp

show_enter_tel proc far stdcall uses ds dx
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_enter_tel
    int 21h

    ret
show_enter_tel endp

show_success proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_success
    int 21h
    ret
show_success endp

show_failed proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_failed
    int 21h
    ret
show_failed endp

show_not_found proc far stdcall uses ds dx 
    mov ax, mydata
    mov ds, ax

    mov ah, 09h
    mov dx, offset g_not_found
    int 21h
    ret
show_not_found endp


mycode ends
end