ifndef DEFINEINC
DEFINEINC equ 1

Cont struc 
    m_nID  dw 0
    m_sName  db 50 dup(?)
    m_sTel db 13 dup(?)
    m_sAddr db 255 dup(?)
Cont ends

endif