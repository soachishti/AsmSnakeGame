SetDirection PROC, R:BYTE, L:BYTE, U:BYTE, D:BYTE
    mov dl, R
    mov RIGHT, dl
    
    mov dl, L
    mov LEFT, dl
    
    mov dl, U
    mov UP, dl
    
    mov dl, D
    mov DOWN, dl
    ret
SetDirection ENDP

KeySync PROC
    mov ah, 0
    INVOKE GetKeyState, VK_ESCAPE
	.IF ah 
        mov eax, -1
        ret
  	.ENDIF
    
    mov ah, 0
    INVOKE GetKeyState, 'a'
	.IF ah 
        add score, 100
  	.ENDIF
    
    mov ah, 0
    INVOKE GetKeyState, VK_DOWN
	.IF ah && row < maxRow
        INC row
        invoke SetDirection, 0, 0, 0, 1
        ret
  	.ENDIF

    mov ah, 0
	INVOKE GetKeyState, VK_UP
    .IF ah && row > 0
        DEC row
        invoke SetDirection, 0, 0, 1, 0
        ret
    .ENDIF     

    mov ah, 0
	INVOKE GetKeyState, VK_LEFT
    .IF ah && col > 0
        DEC col
        invoke SetDirection, 0, 1, 0, 0
        ret
    .ENDIF  

    mov ah, 0
	INVOKE GetKeyState, VK_RIGHT
    .IF ah && col < maxCol
        INC col
        invoke SetDirection, 1, 0, 0, 0
        ret
    .ENDIF     
    
    .IF RIGHT && col < maxCol
        INC col
    .ELSEIF LEFT && col > 0
        DEC col
    .ELSEIF UP && row > 0
        DEC row
    .ELSEIF DOWN&& row < maxRow
        INC row
    .ENDIF
    
    ret
KeySync ENDP

EatAndGrow PROC
    mov ah, col
    mov al, row
    .IF ah == FoodLoc.x && al == FoodLoc.y
        call GenerateFood
        INC headIndex    ; Bound will be handle in KeySync    
        ADD score, 10
    .ENDIF
    
    ret
EatAndGrow ENDP
    
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
    mov ecx, 0
    mov cl, headIndex   ; current head pos in array
    
    ; Add current x and y axis to SnakeBody
    mov al, col
    mov ah, row
    
    mov SnakeBody[2 * ecx].x, al
    mov SnakeBody[2 * ecx].y, ah
    
    mGotoxy SnakeBody[2 * ecx].x, SnakeBody[2 * ecx].y
    mWrite "*"  
    
    invoke Sleep, SnakeSpeed
      
    mov ecx, 0  
    mov cl, tailIndex
    .IF SnakeBody[2 * ecx].x != 0
        mGotoxy SnakeBody[2 * ecx].x, SnakeBody[2 * ecx].y
        mWrite " " 
    .ENDIF
    
    INC tailIndex
    INC headIndex
    
    .IF tailIndex == maxSnakeSize
        mov tailIndex, 0
    .ENDIF

    .IF headIndex == maxSnakeSize
        mov headIndex, 0
    .ENDIF
    ret
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
    mWrite "Score:  "    
    mov  eax,score
    call WriteInt

    mGotoxy 20, maxRow+1
    mWrite "Name: "
    mWriteStr playerName
    
    mGotoxy 40, maxRow+1    
    mWrite "Speed: "
    mov  eax,SnakeSpeed
    call WriteInt

    mGotoxy 60, maxRow+1
    mWrite "Press ESC to pause!"
    mGotoxy 0,0     
    ret
printInfo ENDP

front PROC
    call ClrScr
    call PrintWall
    mGotoxy 25, 8
    mWrite "SNAKE GAME"
    mGotoxy 25, 9
    mWrite "Developer: Owais (P14-6011)"
    mGotoxy 50, 19
    call WaitMsg
    mGotoxy 0,0     
    ret
front ENDP

mainMenu PROC
    call ClrScr
    call PrintWall

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
        mov SnakeSpeed, 100 
    .ELSEIF tChar == 'n'
        mov SnakeSpeed, 75 
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
    call PrintWall
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
    mGotoxy 0,0      
    ret
pausedView ENDP

gameOverView PROC   
    call Clrscr
    call PrintWall
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
    mov col, 40
    mov row, 10 
    mov headIndex, 3
    mov tailIndex, 0
    mov score, 0
    invoke SetDirection, 1,0,0,0
    ret
ResetData ENDP

GenerateFood PROC
    call Randomize

    call Random32   
    XOR EDX,EDX
    MOV ECX, maxCol - 1
    DIV ECX
    inc dl
    mov FoodLoc.x, dl

    call Random32   
    XOR EDX,EDX
    MOV ECX, maxRow - 1
    DIV ECX
    inc dl
    mov FoodLoc.y, dl
    
    mGotoxy FoodLoc.x, FoodLoc.y
    mov al, foodSign
    call WriteChar
    ret
GenerateFood ENDP

StartGame PROC    
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
StartGame ENDP