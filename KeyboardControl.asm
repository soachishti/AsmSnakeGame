; Remove usage of mWrite
; Highly optimization need in PrintSnake

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
maxSnakeSize  EQU     255
      
GetKeyState PROTO, nVirtKey:DWORD

.data
    col         BYTE    1
    row         BYTE    1
    SnakeBody   BODY    maxSnakeSize DUP(<0,0>)
    currSize    BYTE    3   
    currIndex   BYTE    3   ; must be same as currSize
    tail        BYTE    0
    tmp         BYTE    0
.code

InitSnakeBody PROC

    mov ah, 40  ;col
    mov al, 10  ;row
    mov ECX, 0
    mov cl, currSize
    .WHILE cl
        MOV esi, OFFSET SnakeBody
        
        MOV BYTE PTR SnakeBody[2 * ecx].x, ah
        MOV BYTE PTR SnakeBody[2 * ecx].y, al
        
        mGotoxy SnakeBody[2 * ecx].x, SnakeBody[2 * ecx].y
        mWrite "*"
        
        DEC ah
        DEC ECX
    .ENDW
    
InitSnakeBody ENDP

addBody PROC, X:BYTE, Y:BYTE
    ; mov dl, SnakeBody[2 * ecx - 1].x + X
    ; mov SnakeBody[2 * ecx].x, dl
    ; mov dl, SnakeBody[2 * ecx - 1].y + Y
    ; mov SnakeBody[2 * ecx].y, dl
    ; INC ECX
    ; INC currIndex
    ; .IF currIndex == maxSnakeSize - 1 
        ; MOV currIndex, 0
    ; .ENDIF  
addBody ENDP

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
    .IF col == 0 || col == maxCol || row == 0 || row == maxRow
        mov EAX, 0
    .ENDIF
    mov EAX, 1
isGameOver ENDP

printSnake2 PROC
    ; mov EAX, 0 
    ; mov ECX, 0
    ; mov al, currSize
    ; mov cl, currIndex

    ; mGotoxy SnakeBody[2 * ECX - 1].x, SnakeBody[2 * ECX - 1].y ; Erase last elements
    ; mWrite " "
    
    ; .WHILE eax
        ; mGotoxy SnakeBody[2 * ECX].x, SnakeBody[2 * ECX].y
        ; mWrite "*"
        
        ; .IF ECX == 0 ; Moving to end of array to continue from there
            ; mov ECX, maxSnakeSize
        ; .ELSE
            ; DEC ecx
        ; .ENDIF
        
        ; DEC eax
    ; .ENDW
printSnake2 ENDP    
    
printSnake PROC    
    mGotoxy col, row
    mWrite "*"    
      
    invoke Sleep, SnakeSpeed

    mGotoxy col, row
    mWrite " " 
printSnake ENDP

printInfo PROC
    mGotoxy 0, maxRow + 1
    mWrite "Score: 123         Name: Owais       Level: Intermidiate       Press P to pause!"
    mGotoxy 0,0
printInfo ENDP

main PROC
    ; call getInfo ; front page
    call PrintWall
    call InitSnakeBody
    call printInfo

    ; ret
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