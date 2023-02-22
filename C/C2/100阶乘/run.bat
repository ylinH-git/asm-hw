# run.bat文件
# 删除之前的文件
del *.obj
del *.exe

# 重新编译 /W4 输出警告 /WX 把警告当作错误阻止编译
cl /c /W3 /WX main.c
link main.obj
pause
main.exe