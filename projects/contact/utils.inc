ifndef UTILSINC
UTILSINC equ 1

memcpy proto far stdcall dest:word, src:word, bufSize:word
memcmp proto far stdcall lhs:word, rhs:word, count:word
memset proto far stdcall pdest:word, value:word, count:word

strlen proto far stdcall pStr:word
strcpy proto far stdcall destination:word, source:word
strcmp proto far stdcall str1:word, str2:word
strcat proto far stdcall destination:word, source:word

; 清屏函数
clsfn proto far stdcall

; 清空指定的一片内存
clear_buf proto far stdcall pdest:word, count:word

; 创建并写入文件
write_file proto far stdcall pFileName:word, pBuf:word, count: word

; 打开并读取文件
read_file proto far stdcall pFileName:word, pBuf:word, count: word

; 转换为数字(仅支持0~ffff)
to_num proto far stdcall numStr:word, count: word

; 替换得到输入后的回车符为$
del_dh proto far stdcall input:word

; 数字变成字符串(仅支持0~ffffh之间)
num_to_str proto far stdcall num:word, numStr:word

endif