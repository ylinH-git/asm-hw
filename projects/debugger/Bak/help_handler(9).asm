.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szHelpHeader 	db "������  ������   	Ӣ��˵��         	����1		����2			����3					", 0dh, 0ah, 0
	g_szT 			db "t       �������� 	    Step                                 									", 0dh, 0ah, 0
	g_szP 			db "p       �������� 	    Step Go                               									", 0dh, 0ah, 0
	g_szG 			db "g       ���� 	        Go             		��/��ַ                         					", 0dh, 0ah, 0
	g_szU 			db "u       ����� 	    	Display Asmcode  	��/��ַ                         					", 0dh, 0ah, 0
	g_szDD 			db "dd      ��ʾ�ڴ����� 	Display Data     	��/��ַ                         					", 0dh, 0ah, 0
	g_szR			db "r       �Ĵ��� 	    	Register                                								", 0dh, 0ah, 0
	g_szE 			db "e       �޸��ڴ����� 	Edit Data        	��/��ַ                            					", 0dh, 0ah, 0
	g_szBP 			db "bp      һ��ϵ�	    Break Point      	��ַ                       							", 0dh, 0ah, 0
	g_szBPL			db "bpl     һ��ϵ��б� 	Bp List                                									", 0dh, 0ah, 0
	g_szBPC			db "bpc    	ɾ��һ��ϵ�    Clear bp            ��ַ                   								", 0dh, 0ah, 0
	g_szBH 			db "bh      Ӳ���ϵ� 	    Bp Hard             ��ַ		�ϵ㳤��(1,2,4)	e(ִ��)/w(д��)/a(����)	", 0dh, 0ah, 0
	g_szBHL			db "bhl     Ӳ���ϵ��б�	Bp Hard List                              								", 0dh, 0ah, 0
	g_szBHC			db "bhc     ɾ��Ӳ���ϵ� 	Clear Bp Hard       ���                       							", 0dh, 0ah, 0
	g_szBM 			db "bm      �ڴ�ϵ� 	    Bp Memory           ��ַ    	r(��)/w(д)     ����          			", 0dh, 0ah, 0
	g_szBML			db "bml     �ڴ�ϵ��б� 	Bp Memory List                               							", 0dh, 0ah, 0
	g_szBMPL		db "bmpl    ��ҳ�ϵ��б�    Bp Page List                        									", 0dh, 0ah, 0
	g_szBMC			db "bmc     ɾ���ڴ�ϵ�    Clear bp Memory     ���                        						", 0dh, 0ah, 0
	g_szLS 			db "ls      ����ű� 	    Load Script                                								", 0dh, 0ah, 0
	g_szES			db "es      �����ű� 	    Export Script                           								", 0dh, 0ah, 0
	g_szQ 			db "q      	�˳����� 	    Quit                               										", 0dh, 0ah, 0
	g_szML 			db "ml      �鿴ģ�� 	    Module List                                								", 0dh, 0ah, 0
	g_szH 			db "h 		����			Help                                									", 0dh, 0ah, 0
	g_szTRACE 		db "trace  	�Զ����� 	    Trace               ��ַ                 								", 0dh, 0ah, 0
	g_szDUMP		db "dump    Dump 	      	Dump                            										", 0dh, 0ah, 0
.code 

ShowHelper proc
	invoke crt_printf, offset g_szHelpHeader
	invoke crt_printf, offset g_szT
	invoke crt_printf, offset g_szP
	invoke crt_printf, offset g_szG
	invoke crt_printf, offset g_szU
	invoke crt_printf, offset g_szDD
	invoke crt_printf, offset g_szR
	invoke crt_printf, offset g_szE
	invoke crt_printf, offset g_szBP
	invoke crt_printf, offset g_szBPL
	invoke crt_printf, offset g_szBPC
	invoke crt_printf, offset g_szBH
	invoke crt_printf, offset g_szBHL
	invoke crt_printf, offset g_szBHC
	invoke crt_printf, offset g_szBM
	invoke crt_printf, offset g_szBML
	invoke crt_printf, offset g_szBMPL
	invoke crt_printf, offset g_szBMC
	invoke crt_printf, offset g_szLS
	invoke crt_printf, offset g_szES
	invoke crt_printf, offset g_szQ
	invoke crt_printf, offset g_szML
	invoke crt_printf, offset g_szH
	invoke crt_printf, offset g_szTRACE
	invoke crt_printf, offset g_szDUMP
	ret
ShowHelper endp


end