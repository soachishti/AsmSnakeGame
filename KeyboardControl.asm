; Remove usage of mWrite
; Highly optimization need in PrintSnake

TITLE Keyboard Toggle Keys             (Keybd.asm)

INCLUDE Irvine32.inc
INCLUDE Macros.inc
INCLUDELIB user32.lib


AXIS STRUCT 
    x BYTE 0
    y BYTE 0
AXIS ENDS

VK_LEFT		  EQU		000000025h
VK_UP		  EQU		000000026h
VK_RIGHT	  EQU		000000027h
VK_DOWN		  EQU		000000028h
maxCol        EQU     79
maxRow        EQU     20
wallTop       EQU     "================================================================================"
wallLeft      EQU     '|'

maxSnakeSize  EQU     255
      
GetKeyState PROTO, nVirtKey:DWORD

.data
    SnakeSpeed  DWORD   25
    playerName  BYTE    13+1 DUP (?)
    col         BYTE    1
    row         BYTE    1
    SnakeBody   AXIS   maxSnakeSize DUP(<0,0>)
    currSize    BYTE    3   
    currIndex   BYTE    3   ; must be same as currSize
    tmp         BYTE    0
    score       DWORD   0
    tChar       BYTE    0
    FoodLoc     AXIS    <0,0>
.code
INCLUDE procedures.inc




main PROC
    
    ;call pausedView    ; Return in EAX 
    
    call front ; front page
    StartFromMenu:
    call mainMenu
    call InitSnakeBody
    
    Restart:
    call PrintWall
    call printInfo

   
    foreverLoop:   
        call KeySync
        .IF EAX == -1
            jmp GamePaused
        .ENDIF

        call isGameOver
        .IF EAX == 1
            jmp GameOver
        .ENDIF
        
        call printSnake    

       jmp foreverLoop
   
    GamePaused:
        invoke Sleep, 500
        call pausedView
        mov tChar, al
        .IF tChar == 0      ;Resume
            jmp Restart
        .ELSEIF tChar == 1  ;Restart
            call ResetData
            jmp Restart
        .ELSEIF tChar == 2
            jmp StartFromMenu
        .ELSE 
            call ClrScr
            invoke ExitProcess, 0
        .ENDIF
        jmp foreverLoop
   
    GameOver:
        invoke Sleep, 500
        ; Do stuff for after gameover
        call gameOverView 
        mov tChar, al           ; if we dont store value in memory .IF will change EAX while processing

        .IF tChar == 0      ; Restart
            call ResetData
            jmp Restart
            ret
        .ELSEIF tChar == 1  ; Main menu
            jmp StartFromMenu
            ; Main Menu
        .ELSE
            call ClrScr
            invoke ExitProcess, 0
        .ENDIF
        jmp foreverLoop
    
	exit
main ENDP
END main