.386
.model flat, stdcall  ;32 bit memory model
option casemap :none  ;case sensitive

include global.inc
include utils.inc

.data
	g_szHelpHeader 	db "命令码  命令名   	英文说明         	参数1		参数2			参数3					", 0dh, 0ah, 0
	g_szT 			db "t       单步步入 	    Step                                 									", 0dh, 0ah, 0
	g_szP 			db "p       单步步过 	    Step Go                               									", 0dh, 0ah, 0
	g_szG 			db "g       运行 	        Go             		无/地址                         					", 0dh, 0ah, 0
	g_szU 			db "u       反汇编 	    	Display Asmcode  	无/地址                         					", 0dh, 0ah, 0
	g_szDD 			db "dd      显示内存数据 	Display Data     	无/地址                         					", 0dh, 0ah, 0
	g_szR			db "r       寄存器 	    	Register                                								", 0dh, 0ah, 0
	g_szE 			db "e       修改内存数据 	Edit Data        	无/地址                            					", 0dh, 0ah, 0
	g_szBP 			db "bp      一般断点	    Break Point      	地址                       							", 0dh, 0ah, 0
	g_szBPL			db "bpl     一般断点列表 	Bp List                                									", 0dh, 0ah, 0
	g_szBPC			db "bpc    	删除一般断点    Clear bp            地址                   								", 0dh, 0ah, 0
	g_szBH 			db "bh      硬件断点 	    Bp Hard             地址		断点长度(1,2,4)	e(执行)/w(写入)/a(访问)	", 0dh, 0ah, 0
	g_szBHL			db "bhl     硬件断点列表	Bp Hard List                              								", 0dh, 0ah, 0
	g_szBHC			db "bhc     删除硬件断点 	Clear Bp Hard       序号                       							", 0dh, 0ah, 0
	g_szBM 			db "bm      内存断点 	    Bp Memory           地址    	r(读)/w(写)     长度          			", 0dh, 0ah, 0
	g_szBML			db "bml     内存断点列表 	Bp Memory List                               							", 0dh, 0ah, 0
	g_szBMPL		db "bmpl    分页断点列表    Bp Page List                        									", 0dh, 0ah, 0
	g_szBMC			db "bmc     删除内存断点    Clear bp Memory     序号                        						", 0dh, 0ah, 0
	g_szLS 			db "ls      导入脚本 	    Load Script                                								", 0dh, 0ah, 0
	g_szES			db "es      导出脚本 	    Export Script                           								", 0dh, 0ah, 0
	g_szQ 			db "q      	退出程序 	    Quit                               										", 0dh, 0ah, 0
	g_szML 			db "ml      查看模块 	    Module List                                								", 0dh, 0ah, 0
	g_szH 			db "h 		帮助			Help                                									", 0dh, 0ah, 0
	g_szTRACE 		db "trace  	自动跟踪 	    Trace               地址                 								", 0dh, 0ah, 0
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