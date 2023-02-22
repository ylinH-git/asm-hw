ifndef UTILSINC
UTILSINC equ 1

memcpy proto far stdcall dest:word, src:word, bufSize:word
memcmp proto far stdcall lhs:word, rhs:word, count:word
memset proto far stdcall pdest:word, value:word, count:word

strlen proto far stdcall pStr:word
strcpy proto far stdcall destination:word, source:word
strcmp proto far stdcall str1:word, str2:word
strcat proto far stdcall destination:word, source:word

endif