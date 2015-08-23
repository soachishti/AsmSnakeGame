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
    call StartGame   
	ret
main ENDP
END main