ifndef SEARCHFNINC
SEARCHFNINC equ 1

search_by_id proto far stdcall input:word, contInfo: word, contNum:word, buff:word
search_by_name proto far stdcall input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
search_by_addr proto far stdcall input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
search_by_tel proto far stdcall input:word, contInfo: word, contNum:word, buff:word, printBuf:word, rnBuf:word
show_conf_info proto far stdcall contInfo:word, printBuf:word, printRn:word
endif