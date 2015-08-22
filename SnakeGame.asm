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
VK_ESCAPE     EQU		00000001bh
maxCol        EQU       79
maxRow        EQU       20
wallTop       EQU       "================================================================================"
wallLeft      EQU       '|'
maxSnakeSize  EQU       255
      
GetKeyState PROTO, nVirtKey:DWORD

.data
    SnakeSpeed  DWORD   75
    playerName  BYTE    13+1 DUP (?)
    col         BYTE    40
    row         BYTE    10
    SnakeBody   AXIS    maxSnakeSize DUP(<0,0>)
    headIndex   BYTE    3   ; must be same as currSize
    tailIndex   BYTE    0   ; must be same as currSize
    tmp         DWORD   0
    score       DWORD   0
    tChar       BYTE    0
    FoodLoc     AXIS    <0,0>
    LEFT        BYTE    0
    RIGHT       BYTE    1   ; Start moving to right after starting 
    UP          BYTE    0
    DOWN        BYTE    0
    foodSign    BYTE    '@'

.code
INCLUDE Procedures.inc

main PROC    
    call front ; front page
    
    StartFromMenu:
    call mainMenu
    
    Restart:
    call ClrScr
    call GenerateFood
    call PrintWall
   
    foreverLoop:   
        call EatAndGrow
    
        call KeySync
        .IF EAX == -1
            jmp GamePaused
        .ENDIF

        call isGameOver
        .IF EAX == 1
            jmp GameOver
        .ENDIF
        
        call printSnake2  
        call printInfo
        INC score
       jmp foreverLoop
   
    GamePaused:
        invoke Sleep, 100
        call pausedView
        mov tChar, al
        .IF tChar == 0      ;Resume
            jmp Restart
        .ELSEIF tChar == 1  ;Restart
            call ResetData
            jmp Restart
        .ELSEIF tChar == 2
            call ResetData
            jmp StartFromMenu
        .ELSE 
            call ClrScr
            invoke ExitProcess, 0
        .ENDIF
        jmp foreverLoop
   
    GameOver:
        invoke Sleep, 100
        ; Do stuff for after gameover
        call gameOverView 
        mov tChar, al           ; if we dont store value in memory .IF will change EAX while processing

        .IF tChar == 0      ; Restart
            call ResetData
            jmp Restart
            ret
        .ELSEIF tChar == 1  ; Main menu
            call ResetData
            jmp StartFromMenu
            ; Main Menu
        .ELSE
            call ClrScr
            invoke ExitProcess, 0
        .ENDIF
        jmp foreverLoop
    
	ret
main ENDP
END main