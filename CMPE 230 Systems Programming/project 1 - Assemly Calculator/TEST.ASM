
mov bx,00
mov cx,16
mov di,0						;"number reading" flag


process:

    ;INPUT 
    mov ah,01h  				;reads a character input
    int 21h 

    
    ;ENTER_1 
    cmp al,13					;if the incoming character is "enter" jumps to output2
    je output2


    ;SPACE 
    cmp al,32					;controls if incoming character is a "space" or not
    jne not_space 
    cmp di,1					;controls "number reading" flag 
    jne process				
    mov di,0					;sets di to 0 (number token ended)
    push bx						;pushes number to stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_space:

    ;ADD 
    cmp al,43					;Controls if its an addition operation or not
    jne not_add
    pop bx 						;pops two numbers
    pop dx
    add bx,dx					;sums them up
    push bx 					;pushes result to the stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_add:

    ;MUL 
    cmp al,42					;Controls if its an multiplication operation or not
    jne not_mul
    pop bx 						;pops two numbers
    pop ax
    mul bx						;multiplies those numbers
    push ax						;pushes result to the stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_mul:


    ;DIV 
    cmp al,47					;Controls if its an division operation or not
    jne not_div
    pop bx 						;pops divisor
    pop ax						;pops divident
    div bx						;divides divident to divisor
    push ax						;pushes quotient (result) to stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_div:

    ;AND 
    cmp al,38					;Controls if its an and operation or not
    jne not_and
    pop bx 						;pops two numbers
    pop ax
    and ax,bx					;applies and operation
    push ax						;pushes result to the stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_and:


    ;ENTER_2 
    output2:
    cmp al,13					;to create a longer jump
    je output 


    ;OR 
    cmp al,124					;Controls if its an xor operation or not
    jne not_or
    pop bx 						;pops two numbers
    pop ax
    or ax,bx					;applies or operation
    push ax						;pushes result to the stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_or:

    ;XOR 
    cmp al,'^'					;Controls if its an xor operation or not
    jne not_xor
    pop bx 						;pops two numbers
    pop ax
    xor ax,bx					;applies xor operation
    push ax						;pushes result to the stack
    mov bx,00					;sets bx back to 00
    jmp process
    not_xor:

    ;HEXA CHARACTER
    cmp al,70					;Controls if character in the interval of A-F or not
    jg not_hexa
    cmp al,64
    jle not_hexa
    mov ah,00h
    sub ax,'A'					;Deriving numerical value from the ASCII
    add ax,10
    mov dx,ax					;register manuevers
    mov ax,bx
    mov bx,dx
    mul cx						;creating a space for new digit
    add ax,bx					;add the new digit
    mov bx,ax					;store current number at bx register
    mov di,1					;sets "number reading" flag to 1
    jmp process
    not_hexa:

   


    ;ASCII TO DECIMAL
    sub al,'0' 					;stripping ASCII
    mov ah,00h

    ;ADD CHAR TO NUMBER 
    mov dx,ax					;register maneuvers
    mov ax,bx
    mov bx,dx
    mul cx						;creating a space for the new digit
    add ax,bx					;add the new digit
    mov bx,ax					;store current number at bx register
    mov di,1					;sets "number reading" flag to 1
    jmp process




output: 

    pop ax						;pops the number(the last number) in the stack
    push 0						;pushes 0 to stack

    
    out_stack:

        mov dx, ax				;creates a copy of the value that is in ax
        and dx, 0fh				;takes last 4 bits of the number (last digit in hexadecimal)
        mov si, dx      		;assigns dx to si
        mov dl, [convert + si]  ;derives corresponding ASCII character               
        push dx					;pushes this character to stack

        shr ax, 4				;shifts 4 bits to the right (divides by 16)
        cmp ax,0				;checks if number is processed completely
        jne out_stack

    print:
        
        ;PRINTS OUTPUT 
        pop dx 					;pops one character from the stack
        cmp dx,0				;controls if it is zero, if so printing process ends
        je exit	
        mov ah,02h				;print to console
        int 21h 
        jmp print

        

         

exit:

     

    mov ah,4Ch
    mov al,00h 					;exit code with 00
    int 21h 					;terminates program

convert:
    db '0123456789ABCDEF'