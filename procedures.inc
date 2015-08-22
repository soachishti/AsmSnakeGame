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
    ret
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
    INVOKE GetKeyState, 'p'
	.IF ah 
        mov eax, -1
        ret
  	.ENDIF

    mov ah, 0
    INVOKE GetKeyState, VK_DOWN
	.IF ah && row < maxRow 
        INC row
        ret
  	.ENDIF

    mov ah, 0
	INVOKE GetKeyState, VK_UP
    .IF ah && row > 0
        DEC row
        ret
    .ENDIF     

    mov ah, 0
	INVOKE GetKeyState, VK_LEFT
    .IF ah && col > 0 
        DEC col
        ret
    .ENDIF  

    mov ah, 0
	INVOKE GetKeyState, VK_RIGHT
    .IF ah && col < maxCol
        INC col
        ret
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
    .IF col == 0 || row == 0 || col == maxCol || row == maxRow
        mov EAX, 1
        ret
    .ENDIF
    mov EAX, 0 
    ret
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
    ret
printSnake ENDP

printInfo PROC

    mGotoxy 0, maxRow+1
    mWrite "Score: "    
    mov  eax,score
    call WriteInt

    mGotoxy 20, maxRow+1
    mWrite "Name: "
    mWriteStr playerName
    
    mGotoxy 40, maxRow+1    
    mWrite "Speed: "
    mov  eax,SnakeSpeed
    call WriteInt

    mGotoxy 63, maxRow+1
    mWrite "Press P to pause!"
    mGotoxy 0,0     
    ret
printInfo ENDP

front PROC
    call ClrScr
    mGotoxy 30, 8
    mWrite "SNAKE GAME"
    mGotoxy 30, 9
    mWrite "Developer: Owais (P14-6011)"
    mGotoxy 50, 20
    call WaitMsg
    mGotoxy 0,0     
    ret
front ENDP

mainMenu PROC
    call ClrScr

    mGotoxy 30, 5
    mWrite "MAIN MENU"
    mGotoxy 30, 7
    mWrite "Enter Name: "
    mReadStr playerName
    mGotoxy 30, 9
    mWrite "LEVEL"
    mGotoxy 30, 10  
    mWrite "1) Basic   -> b"
    mGotoxy 30, 11 
    mWrite "2) Normal  -> n"
    mGotoxy 30, 12 
    mWrite "3) Advance -> a"
    mGotoxy 30, 13 
    mWrite "4) Expert  -> e"
    mGotoxy 30, 14 
    mWrite "Choice: "
    call ReadChar    
    mov  tChar,al 
    call WriteChar

    .IF tChar == 'b'
        mov SnakeSpeed, 200 
    .ELSEIF tChar == 'n'
        mov SnakeSpeed, 100 
    .ELSEIF tChar == 'a'
        mov SnakeSpeed, 50 
    .ELSEIF tChar == 'e'
        mov SnakeSpeed, 25 
    .ENDIF
    invoke Sleep, 100
    mGotoxy 0,0
    call ClrScr
    ret
mainMenu ENDP

pausedView PROC
    call ClrScr
    mGotoxy 30, 7
    mWrite "GAMED PAUSED"
    mGotoxy 30, 9
    mWrite "OPTIONS"
    mGotoxy 30, 10  
    mWrite "1) Resume    -> r"
    mGotoxy 30, 11  
    mWrite "2) Restart   -> t"
    mGotoxy 30, 12 
    mWrite "3) Main Menu -> m"
    mGotoxy 30, 13 
    mWrite "4) Exit      -> e"
    mGotoxy 30, 14 
    mWrite "Choice: "
    call ReadChar
    mov  tChar,al  
    call WriteChar
    invoke Sleep, 100
    
    .IF tChar == 'r'
        mov EAX, 0
    .ELSEIF tChar == 't'
        mov EAX, 1
    .ELSEIF tChar == 'm'
        mov EAX, 2
    .ELSE
        mov EAX, 3
    .ENDIF
    invoke Sleep, 1000
    mGotoxy 0,0      
    ret
pausedView ENDP

gameOverView PROC   
    call Clrscr
    mGotoxy 30, 7
    mWrite "GAME OVER!!!"
    mGotoxy 30, 9  
    mWrite "Score:"
    mov eax, score
    call WriteInt
    mGotoxy 30, 11  
    mWrite "OPTIONS"
    mGotoxy 30, 12 
    mWrite "1) Restart   -> t"
    mGotoxy 30, 13
    mWrite "2) Main Menu -> m"
    mGotoxy 30, 14 
    mWrite "3) Exit      -> any key"
    mGotoxy 30, 15 
    mWrite "Choice: "
    
    mov eax, 0  
    call ReadChar
    mov  tChar,al  
    call WriteChar
    invoke Sleep, 100
        
    .IF tChar == 't'
        mov eax, 0    
    .ELSEIF tChar == 'm'
        mov eax, 1
    .ELSE
        mov eax, 2
    .ENDIF
        
    mGotoxy 0,0     
    ret 
gameOverView ENDP

ResetData PROC
    mov SnakeSpeed, 25    
    mov col, 1 
    mov row, 1 
    mov currSize, 3
    mov currIndex, 3
    mov score, 0
    ret
ResetData ENDP

GenerateFood PROC
    call Random32   
    XOR EDX,EDX
    MOV ECX, maxCol
    DIV ECX
    mov FoodLoc.x, EDX

    call Random32   
    XOR EDX,EDX
    MOV ECX, maxCol
    DIV ECX
    mov FoodLoc.y, EDX
GenerateFood ENDP
