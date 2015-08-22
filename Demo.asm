INCLUDE irvine32.inc
INCLUDE WIN.inc

includelib user32.lib
includelib kernel32.lib

AXIS STRUCT 
    x BYTE 0
    y BYTE 0
AXIS ENDS

.data
    message BYTE "This file cannot be opened.",0

.code
    main PROC
        mov  eax,OFFSET message
        mov  ebx,0                       ; no caption
        call MsgBox
        ret
    main ENDP
END main
    