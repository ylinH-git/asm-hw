echo off
set path=%path%;D:\masm32\bin
set include=D:\masm32\include
set lib=D:\masm32\lib
echo no

% /coff表示编译的是32位汇编，否则默认是16位的 %
ml /c /coff %1.asm
% subsystem表示需要的子系统是什么，窗口程序就是用windows %
% 这里跟写c时配环境一样 %
link /subsystem:windows %1.obj