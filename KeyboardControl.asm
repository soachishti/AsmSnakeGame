TITLE Keyboard Toggle Keys             (Keybd.asm)

INCLUDE Irvine32.inc
INCLUDE Macros.inc
INCLUDELIB user32.lib

VK_LEFT		EQU		000000025h
VK_UP		EQU		000000026h
VK_RIGHT	EQU		000000027h
VK_DOWN		EQU		000000028h
maxCol      EQU     79
maxRow      EQU     20

GetKeyState PROTO, nVirtKey:DWORD

.data
    col BYTE 0
    row BYTE 0

.code
main PROC

looop:   

    mov ah, 0

    INVOKE GetKeyState, VK_DOWN
	.IF ah && row < maxRow
        ;mWriteLn "DOWN"
        INC row
  	.ENDIF

	INVOKE GetKeyState, VK_UP
    .IF ah && row > 0
        ;mWriteLn "UP"
        DEC row
	.ENDIF     

	INVOKE GetKeyState, VK_LEFT
    .IF ah && col > 0 
        ;mWriteLn "LEFT"
        DEC col
	.ENDIF  

	INVOKE GetKeyState, VK_RIGHT
    .IF ah && col < maxCol
        ;mWriteLn "RIGHT"
        INC col
	.ENDIF     
        


    mov  dl, col        ; column
    mov  dh, row        ; row
    call Gotoxy         ; Change position according to new input
        
    mov  al, '*'          
    call WriteChar      ; Write point on new place
 
    ;mov eax, 0
    ;mov al, col
    ;call WriteInt
    ;mov al, '-'
    ;call WriteChar
    ;mov al, row
    ;call WriteInt
    ;call Crlf
 
    invoke Sleep, 25
    
    
    ; Erase Point
    mov  dl, col        ; column
    mov  dh, row        ; row
    call Gotoxy         ; Change position according to new input
    
    mov  al,' '     
    call WriteChar      ; Remove previous data

    
    
    jmp looop

	exit
main ENDP
END main