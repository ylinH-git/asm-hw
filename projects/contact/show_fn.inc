ifndef SHOWFNINC
SHOWFNINC equ 1

show_main_menu proto far stdcall
show_success proto far stdcall
show_failed proto far stdcall
show_add_menu proto far stdcall addType:word
show_search_menu proto far stdcall
show_enter_id proto far stdcall
show_enter_name proto far stdcall
show_enter_addr proto far stdcall
show_enter_tel proto far stdcall
show_modify_menu proto far stdcall
show_list_title proto far stdcall
show_next proto far stdcall
show_not_found proto far stdcall

endif