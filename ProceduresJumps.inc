; Uses only Jump for conditions

SetDirection PROC, R:BYTE, L:BYTE, U:BYTE, D:BYTE
        ; LEFT, RIGHT, UP, DOWN global variables.
        mov     dl, R
        mov     RIGHT, dl
        
        mov     dl, L
        mov     LEFT, dl
        
        mov     dl, U
        mov     UP, dl
        
        mov     dl, D
        mov     DOWN, dl
        
        ret
SetDirection ENDP

KeySync PROC
        mov     ah, 0
        INVOKE  GetKeyState, VK_ESCAPE
        cmp     ah, 0
        je      l0      ; if ah != 0
        mov     eax, -1
        ret
  	l0:
    
        mov     ah, 0
        INVOKE  GetKeyState, 'a'
        cmp     ah, 0
        je      l1      ; if ah
        add     score, 100
  	l1:
    
        mov     ah, 0
        INVOKE  GetKeyState, VK_DOWN
        cmp     ah, 0   ; ah && row < maxRow
        je      l2 
        cmp     row, maxRow
        jnl     l2   
        INC     row
        invoke  SetDirection, 0, 0, 0, 1
        ret
  	l2:

        mov     ah, 0
        INVOKE  GetKeyState, VK_UP
        cmp     ah, 0   ; ah && row > 0
        je      l3
        cmp     row, 0
        jng     l3  
        DEC     row
        invoke  SetDirection, 0, 0, 1, 0
        ret
    l3:     

        mov     ah, 0
        INVOKE  GetKeyState, VK_LEFT
        cmp     ah, 0   ; ah && col > 0
        je      l4
        cmp     col, 0
        jng     l4 
        DEC     col
        invoke  SetDirection, 0, 1, 0, 0
        ret
    l4:  

        mov ah, 0
        INVOKE  GetKeyState, VK_RIGHT
        cmp     ah, 0   ; ah && col < maxCol
        je      l5
        cmp     col, maxCol
        jnl     l5 
        INC     col
        invoke  SetDirection, 1, 0, 0, 0
        ret
    l5:     
    
    
        ; IF RIGHT && col < maxCol
        cmp     RIGHT, 0
        je      l6
        cmp     col, maxCol
        jnl     l6
        INC     col
    l6:
        ; ELSEIF LEFT && col > 0
        cmp     LEFT, 0 
        je      l7
        cmp     col, 0
        jng     l7
        DEC     col
    l7:
        ; ELSEIF UP && row > 0
        cmp     UP, 0
        je      l8
        cmp     row, 0
        jng     l8
        DEC     row
    l8:
        ; ELSEIF DOWN && row < maxRow
        cmp     DOWN, 0
        je      l9
        cmp     row, maxRow
        jnl     l9
        INC     row
    l9:

        ret
KeySync ENDP

EatAndGrow PROC
        mov     ah, col
        mov     al, row
        ; ah == FoodLoc.x && al == FoodLoc.y
        cmp     ah, FoodLoc.x
        jne     l0
        cmp     al, FoodLoc.y
        jne     l0
        call    GenerateFood
        INC     headIndex    ; Bound will be handle in KeySync    
        ADD     score, 10
    l0:
        ret
EatAndGrow ENDP
    
PrintWall PROC
        mGotoxy     0, 0     
        mWrite      wallTop
        mGotoxy     0, maxRow    
        mWrite      wallTop
        
        mov         cl, maxRow - 1 
    l0:
        ;.while cl
        cmp         cl, 0 
        je          l1
        mGotoxy     0, cl   
        mWrite      wallLeft
        mGotoxy     maxCol, cl
        mWrite      wallLeft
        DEC         cl
        jmp         l0
    l1:
        ret
PrintWall ENDP

isGameOver PROC
        ;IF col == 0 || row == 0 || col == maxCol || row == maxRow
        cmp     col, 0
        je      l1
        cmp     row, 0
        je      l1
        cmp     col, maxCol
        je      l1
        cmp     row, maxRow
        je      l1
        jmp     l2
    l1:
        mov     EAX, 1
        ret
    l2: 
        ; ELSE
        mov     EAX, 0 
        ret
isGameOver ENDP

printSnake2 PROC
        mov     ecx, 0
        mov     cl, headIndex   ; current head pos in array
        
        ; Add current x and y axis to SnakeBody
        mov     al, col
        mov     ah, row
        
        mov     SnakeBody[2 * ecx].x, al
        mov     SnakeBody[2 * ecx].y, ah
        
        mGotoxy SnakeBody[2 * ecx].x, SnakeBody[2 * ecx].y
        mWrite  "*"  
        
        invoke  Sleep, SnakeSpeed
          
        mov     ecx, 0  
        mov     cl, tailIndex
        ; IF SnakeBody[2 * ecx].x != 0
        cmp     SnakeBody[2 * ecx].x, 0 
        je      l0
        mGotoxy SnakeBody[2 * ecx].x, SnakeBody[2 * ecx].y
        mWrite  " " 
    l0:
        
        INC     tailIndex
        INC     headIndex
        
        ; IF tailIndex == maxSnakeSize
        cmp     tailIndex, maxSnakeSize
        jne     l1
        mov     tailIndex, 0
    l1:

        ; IF headIndex == maxSnakeSize
        cmp     headIndex, maxSnakeSize
        jne     l2
        mov headIndex, 0
    l2:
        ret
printSnake2 ENDP    
    
printSnake PROC    
        mGotoxy     col, row
        mWrite      "*"    
      
        invoke      Sleep, SnakeSpeed

        mGotoxy     col, row
        mWrite      " "      
        ret
printSnake ENDP

printInfo PROC
        mGotoxy     0, maxRow + 1
        mWrite      "Score:  "    
        mov         eax, score
        call        WriteInt

        mGotoxy     20, maxRow + 1
        mWrite      "Name: "
        mWriteStr   playerName
        
        mGotoxy     40, maxRow + 1    
        mWrite      "Speed: "
        mov         eax, SnakeSpeed
        call        WriteInt

        mGotoxy     60, maxRow + 1
        mWrite      "Press ESC to pause!"
        mGotoxy     0,0     
        ret
printInfo ENDP

front PROC
        call        ClrScr
        call        PrintWall
        mGotoxy     25, 8
        mWrite      "SNAKE GAME"
        mGotoxy     25, 9
        mWrite      "Developer: Owais (P14-6011)"
        mGotoxy     50, 19
        call        WaitMsg
        mGotoxy     0,0     
        ret
front ENDP

mainMenu PROC
        call        ClrScr
        call        PrintWall

        mGotoxy     30, 5
        mWrite      "MAIN MENU"
        mGotoxy     30, 7
        mWrite      "Enter Name: "
        mReadStr    playerName
        mGotoxy     30, 9
        mWrite      "LEVEL"
        mGotoxy     30, 10  
        mWrite      "1) Basic   -> b"
        mGotoxy     30, 11 
        mWrite      "2) Normal  -> n"
        mGotoxy     30, 12 
        mWrite      "3) Advance -> a"
        mGotoxy     30, 13 
        mWrite      "4) Expert  -> e"
        mGotoxy     30, 14 
        mWrite      "Choice: "
        call        ReadChar    
        mov         tChar, al 
        call        WriteChar

        ; IF tChar == 'b'
        cmp     tChar, 'b'
        jne     l1
        mov     SnakeSpeed, 100
        jmp     l4
    l1:
        ; ELSEIF tChar == 'n'
        cmp     tChar, 'n'
        jne     l2
        mov     SnakeSpeed, 75 
        jmp     l4
    l2:
        ; ELSEIF tChar == 'a'
        cmp     tChar, 'a'
        jne     l3
        mov     SnakeSpeed, 50 
        jmp     l4
    l3:
        ; ELSEIF tChar == 'e'
        cmp     tChar, 'e'
        jne     l4
        mov     SnakeSpeed, 25 
    l4:    
        
        invoke  Sleep, 100
        mGotoxy 0,0
        call    ClrScr
        
        ret
mainMenu ENDP

pausedView PROC
        call        ClrScr
        call        PrintWall
        mGotoxy     30, 7
        mWrite      "GAMED PAUSED"
        mGotoxy     30, 9
        mWrite      "OPTIONS"
        mGotoxy     30, 10  
        mWrite      "1) Resume    -> r"
        mGotoxy     30, 11  
        mWrite      "2) Restart   -> t"
        mGotoxy     30, 12 
        mWrite      "3) Main Menu -> m"
        mGotoxy     30, 13 
        mWrite      "4) Exit      -> e"
        mGotoxy     30, 14 
        mWrite      "Choice: "
        call        ReadChar
        mov         tChar,al  
        call        WriteChar
        invoke      Sleep, 100
    
        ; IF tChar == 'r'
        cmp     tChar, 'r'
        jne     l1
        mov     EAX, 0
        jmp     l4
    l1:
        ; ELSEIF tChar == 't'
        cmp     tChar, 't'
        jne     l2
        mov     EAX, 1
        jmp     l4
    l2:
        ; ELSEIF tChar == 'm'
        cmp     tChar, 'm'
        jne     l3
        mov     EAX, 2
        jmp     l4
    l3:
        ; ELSE
        mov     EAX, 3
    l4:
    
        mGotoxy     0,0      
        ret
pausedView ENDP

gameOverView PROC   
        call        Clrscr
        call        PrintWall
        mGotoxy     30, 7
        mWrite      "GAME OVER!!!"
        mGotoxy     30, 9  
        mWrite      "Score:"
        mov         eax, score
        call        WriteInt
        mGotoxy     30, 11  
        mWrite      "OPTIONS"
        mGotoxy     30, 12 
        mWrite      "1) Restart   -> t"
        mGotoxy     30, 13
        mWrite      "2) Main Menu -> m"
        mGotoxy     30, 14 
        mWrite      "3) Exit      -> any key"
        mGotoxy     30, 15 
        mWrite      "Choice: "
        
        mov         eax, 0  
        call        ReadChar
        mov         tChar,al  
        call        WriteChar
        invoke      Sleep, 100
            
        ; IF tChar == 't'
        cmp     tChar, 't'
        jne     l1
        mov     eax, 0    
        jmp     l3
    l1:
        ; ELSEIF tChar == 'm'
        cmp     tChar, 'm'
        jne     l2  
        mov     eax, 1    
        jmp     l3
    l2:
        mov     eax, 2
    l3:
    
        mGotoxy     0,0     
        ret 
gameOverView ENDP

ResetData PROC    
        mov     col, 40
        mov     row, 10 
        mov     headIndex, 3
        mov     tailIndex, 0
        mov     score, 0
        invoke  SetDirection, 1,0,0,0
        
        ret
ResetData ENDP

GenerateFood PROC
        call    Randomize

        call    Random32   
        XOR     EDX,EDX
        MOV     ECX, maxCol - 1
        DIV     ECX
        inc     dl
        mov     FoodLoc.x, dl

        call    Random32   
        XOR     EDX,EDX
        MOV     ECX, maxRow - 1
        DIV     ECX
        inc     dl
        mov     FoodLoc.y, dl
        
        mGotoxy FoodLoc.x, FoodLoc.y
        mov     al, foodSign
        call    WriteChar
        
        ret
GenerateFood ENDP

StartGame PROC    
        call    front ; front page
    
    StartFromMenu:
        call    mainMenu
    
    Restart:
        call    ClrScr
        call    GenerateFood
        call    PrintWall
    
    foreverLoop:   
        call    EatAndGrow
        call    KeySync
        
        ; IF EAX == -1
        cmp     eax, -1
        jne     l0
        jmp     GamePaused
    l0:

        call    isGameOver
        ; IF EAX == 1
        cmp     eax, 1
        jne     l1
        jmp     GameOver
     l1:
        
        call    printSnake2  
        call    printInfo
        INC     score
        
        jmp     foreverLoop
   
    GamePaused:
        invoke  Sleep, 100
        call    pausedView
        mov     tChar, al
        
        ; IF tChar == 0      ;Resume
        cmp     tChar, 0
        jne     l3
        jmp     Restart
    l3:  
        ; ELSEIF tChar == 1  ;Restart
        cmp     tChar, 1
        jne     l4
        call    ResetData
        jmp     Restart
    l4:
        ; ELSEIF tChar == 2     ; Main Menu
        cmp     tChar, 2
        jne     l5
        call    ResetData
        jmp     StartFromMenu
    l5:    
        ; ELSE
        call    ClrScr
        invoke  ExitProcess, 0
       
        jmp     foreverLoop
   
    GameOver:
        invoke  Sleep, 100
        ; Do stuff for after gameover
        call    gameOverView 
        mov     tChar, al           ; if we dont store value in memory .IF will change EAX while processing

        ; IF tChar == 0      ; Restart
        cmp     tChar, 0
        jne     l6
        call    ResetData
        jmp     Restart
        ret
    l6:
        ; ELSEIF tChar == 1  ; Main menu
        cmp     tChar, 1
        jne     l7
        call    ResetData
        jmp     StartFromMenu   ; Main Menu
    l7:
        ; ELSE
        call    ClrScr
        invoke  ExitProcess, 0

        jmp foreverLoop
        
        ret
StartGame ENDP