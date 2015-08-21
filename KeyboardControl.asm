TITLE Keyboard Toggle Keys             (Keybd.asm)

INCLUDE Irvine32.inc
INCLUDE Macros.inc
INCLUDELIB user32.lib

BODY STRUCT 
    x BYTE 0
    y BYTE 0
BODY ENDS

VK_LEFT		  EQU		000000025h
VK_UP		  EQU		000000026h
VK_RIGHT	  EQU		000000027h
VK_DOWN		  EQU		000000028h
maxCol        EQU     79
maxRow        EQU     20
wallTop       EQU     "================================================================================"
wallLeft      EQU     '|'
SnakeSpeed    EQU     25
maxSnakeSize  EQU     100
      
GetKeyState PROTO, nVirtKey:DWORD

.data
    col         BYTE    1
    row         BYTE    1
    SnakeBody   BODY    10 DUP(<0,0>)
    currSize    BYTE    3
.code

InitSnakeBody PROC
    mov cl, currSize
    xor ecx, ecx
    .WHILE cl
        MOV esi, OFFSET SnakeBody
        XOR ch, ch
        MOV BODY PTR [esi + 2 * ECX], 1
        DEC ECX
    .ENDW
InitSnakeBody ENDP


KeySync PROC
    mov ah, 0
    INVOKE GetKeyState, VK_DOWN
	.IF ah && row < maxRow 
        INC row
  	.ENDIF

	INVOKE GetKeyState, VK_UP
    .IF ah && row > 0
        DEC row
	.ENDIF     

	INVOKE GetKeyState, VK_LEFT
    .IF ah && col > 0 
        DEC col
	.ENDIF  

	INVOKE GetKeyState, VK_RIGHT
    .IF ah && col < maxCol
        INC col
	.ENDIF     
    ret
KeySync ENDP

PrintWall PROC
    mGotoxy 0, 0     
    mWrite wallTop
    mGotoxy 0, maxRow    
    mWrite wallTop
        
    mov cl, maxRow - 1 
    .while cl
        mGotoxy 0, cl   
        mWrite wallLeft
        mGotoxy maxCol, cl
        mWrite wallLeft
        DEC cl
    .endw
    ret
PrintWall ENDP

isGameOver PROC
    .IF col == 0 || col == maxCol || row == 0|| row == maxRow
        mov EAX, 0
    .ENDIF
    mov EAX, 1
isGameOver ENDP

printSnake PROC
    mGotoxy col, row
    mWrite "*"    
      
    invoke Sleep, SnakeSpeed

    mGotoxy col, row
    mWrite " " 
printSnake ENDP

main PROC
    call PrintWall

    foreverLoop:   
        call KeySync
        call isGameOver
        .IF EAX
            jmp GameOver
        .ENDIF
        
        call printSnake        

        jmp foreverLoop
    
    GameOver:
        ; Do stuff for after gameover
        jmp foreverLoop
    
	exit
main ENDP
END main