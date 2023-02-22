ifndef MODIFYINC
MODIFYINC equ 1

modify_contact proto far stdcall input:word, contInfo: word, contNum:word, buff:word, pFile:word

endif