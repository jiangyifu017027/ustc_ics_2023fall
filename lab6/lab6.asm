; Unfortunately we have not YET installed Windows or Linux on the LC-3,
; so we are going to have to write some operating system code to enable
; keyboard interrupts. The OS code does three things:
;
;    (1) Initializes the interrupt vector table with the starting
;        address of the interrupt service routine. The keyboard
;        interrupt vector is x80 and the interrupt vector table begins
;        at memory location x0100. The keyboard interrupt service routine
;        begins at x1000. Therefore, we must initialize memory location
;        x0180 with the value x1000.
;    (2) Sets bit 14 of the KBSR to enable interrupts.
;    (3) Pushes a PSR and PC to the system stack so that it can jump
;        to the user program at x3000 using an RTI instruction.

        .ORIG x800
        ; (1) Initialize interrupt vector table.
        LD R0, VEC
        LD R1, ISR
        STR R1, R0, #0

        ; (2) Set bit 14 of KBSR.
        LDI R0, KBSR
        LD R1, MASK
        NOT R1, R1
        AND R0, R0, R1
        NOT R1, R1
        ADD R0, R0, R1
        STI R0, KBSR

        ; (3) Set up system stack to enter user space.
        LD R0, PSR
        ADD R6, R6, #-1
        STR R0, R6, #0
        LD R0, PC
        ADD R6, R6, #-1
        STR R0, R6, #0
        ; Enter user space.
        RTI
        
VEC     .FILL x0180
ISR     .FILL x1000
KBSR    .FILL xFE00
MASK    .FILL x4000
PSR     .FILL x8002
PC      .FILL x3000
        .END

        .ORIG x3000
        
        ; *** Begin user program code here ***
        ;实现不断输出
LOOP	LEA R0, STUID		
		PUTS
		AND R0, R0, #0
		LD R0, DELAY
SLEEP	ADD R0, R0, #-1
		BRp SLEEP
		
		BRnzp LOOP


DELAY	.FILL #25000   ;让输出速度慢一些
STUID	.STRINGZ "PB21020706 "
        ; *** End user program code here ***
        .END

        .ORIG x3FFF

FACT_N  .FILL xFFFF

        .END

        .ORIG x1000
        ; *** Begin interrupt service routine code here ***
        ST R0, SAVE_R0
        ST R1, SAVE_R1
        ST R2, SAVE_R2
        ST R3, SAVE_R3
        ST R4, SAVE_R4
        ST R5, SAVE_R5
        ST R6, SAVE_R6
        ST R7, SAVE_R7
        
        LD R6,STACK;压栈

        LD R0, NEWLINE
        OUT
        GETC ;读取键盘结果
        OUT
        ST R0, NUM
        LD R1, ANSWER
        STR R0, R1, #0
        
        ADD R5, R0, #0
        LD  R3, ASCII_0
        ADD R0, R0, R3    ;n-'0'
        ADD R4, R0, #0
        BRn ERROR_1
        LD  R3, ASCII_9     
        ADD R0, R5, R3    ;n-'9'
        BRp ERROR_1
        
        LEA R0, OUTPUT_2  ;判断键盘输入是一个数字
        PUTS
        
        LD R0, NEWLINE
        OUT
        
        LD R0, NUM
        
        LD  R3, ASCII_7
        ADD R0, R0, R3
        BRp ERROR_2
        
        ADD R0, R4, #0
        BR MAIN
        
ERROR_1 LEA R0, OUTPUT_1
        PUTS
        BR FINISH
ERROR_2 LD R0, NUM
        OUT
        LEA R0, OUTPUT_3
        PUTS
        LD R0, NEWLINE
        OUT
        HALT
FINISH  LD R0, NEWLINE
        OUT
        RTI
       
MAIN    ADD R6,R6,#-1
        STR R0,R6,#0
        JSR FACT
        ;output
        LD R0, NUM
        OUT
        LEA R0, OUTPUT_4
        PUTS
        
        LD R0, OFF
        LD R2, NETH
        
CHANGE_1   ADD R1, R2, R1
           BRn RECHAN_1
           ADD R0, R0, #1
           BRnzp CHANGE_1
RECHAN_1   OUT     ; 千位
           LD R3, POTH
           ADD R1, R1, R3
           LD R0, OFF
           LD R2, NEGH
CHANGE_2   ADD R1, R2, R1
           BRn RECHAN_2
           ADD R0, R0, #1
           BRnzp CHANGE_2
RECHAN_2   OUT    ; 百位
           LD R3, POSH
           ADD R1, R1, R3
           LD R0, OFF
           LD R2, NEGS
CHANGE_3   ADD R1, R2, R1
           BRn RECHAN_3
           ADD R0, R0, #1
           BRnzp CHANGE_3
RECHAN_3   OUT
           AND R3, R3, #0
           ADD R3, R3, #10
           ADD R1, R1, R3
           LD R0, OFF
           ADD R0, R0, R1
           OUT
           
        LD R0, SAVE_R0
        LD R1, SAVE_R1
        LD R2, SAVE_R2
        LD R3, SAVE_R3
        LD R4, SAVE_R4
        LD R5, SAVE_R5
        LD R6, SAVE_R6
        LD R7, SAVE_R7

HALT

FACT ADD R6, R6, #-1
     STR R7, R6, #0 
     ADD R6, R6, #-1
     STR R0, R6, #0 
     ADD R6, R6, #-1
     STR R2, R6, #0 
     AND R2, R0, #-2
     BRnp SKIP ; if n=0,1
     AND R1, R1, #0
     ADD R1, R1, #1
     BRnzp DONE
SKIP ADD R0, R0, #-1  ; R0 = n-1
     JSR FACT ; R1 = (n-1)!
     AND R2, R2, #0
     ADD R2, R0, #0
     AND R3, R3, #0
     ADD R3, R1, #0
    LOOP_PLUS ADD R1, R1, R3
              ADD R2, R2, #-1
              BRp LOOP_PLUS
              
DONE LDR R2, R6, #0

     ADD R6, R6, #1
     LDR R0, R6, #0
     ADD R6, R6, #1
     LDR R7, R6, #0
     ADD R6, R6, #1
     RET

;
ASCII_0 .FILL xFFD0
ASCII_7 .FILL xFFC9
ASCII_9 .FILL xFFC7 
OFF .FILL   x0030
STACK .FILL x6000
NEWLINE .FILL   x000A
OUTPUT_1 .STRINGZ " is not a decimal digit. "
OUTPUT_2 .STRINGZ " is a decimal digit. "
OUTPUT_3 .STRINGZ "! is too large for LC-3. "
OUTPUT_4 .STRINGZ "! = "
NETH  .FILL xFC18
NEGH  .FILL	xFF9C
POSH  .FILL	x0064
POTH  .FILL x03E8
NEGS  .FILL xFFF6
ANSWER .FILL x3FFF
NUM   .BLKW 1
SAVE_R0 .BLKW 1
SAVE_R1 .BLKW 1
SAVE_R2 .BLKW 1
SAVE_R3 .BLKW 1
SAVE_R4 .BLKW 1
SAVE_R5 .BLKW 1
SAVE_R6 .BLKW 1
SAVE_R7 .BLKW 1
; *** End interrupt service routine code here ***
    .END