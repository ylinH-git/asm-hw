# run.bat�ļ�
# ɾ��֮ǰ���ļ�
del *.obj
del *.exe

# ���±��� /W4 ������� /WX �Ѿ��浱��������ֹ����
cl /c /W3 /WX main.c
link main.obj
pause
main.exe