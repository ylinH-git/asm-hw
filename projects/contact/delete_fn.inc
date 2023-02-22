ifndef DELETEFNINC
DELETEFNINC equ 1

del_contact proto far stdcall contInfo: word, contNum:word, buff:word, buffEnd:word, pFile:word

endif