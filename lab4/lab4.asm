.ORIG x3000

AND R0, R0, #0 ;初始化
AND R1, R1, #0
AND R2, R2, #0

LD R0, NUMBER  ;R0作为指针
LDR R1, R0, #0 ;R1作为位数大小

AND R3, R3, #0 ;R3作为辅助变量
ADD R3, R3, #1

AND R4, R4, #0 ;R4作为辅助变量
ADD R4, R4, R1

AND R6, R6, #0

LOOP ADD R2, R2, R3 ;R2作为最后的辅助判断条件
     ADD R3, R3, R3 ;R2为全为1
     ADD R4, R4, #-1
     BRp LOOP

AND R3, R3, #0
ADD R3, R3, #1
ADD R4, R1, #0
AND R7, R7, #0 ;R7作为指针
     
LONG AND R5, R3, #1
     BRp FIRST
     AND R5, R3, #2
     BRp SECOND
     
DEBUG

ADD R0, R0, R7
ADD R0, R0, #1

ADD R7, R2, #0 ;R7为111...

NOT R2, R2
ADD R2, R2, #1
   
AND R3, R3, #0
ADD R3, R3, #1 ;R3作为辅助变量

AND R4, R4, #0 ;R4作为最后的输出变量

ADD R0, R0, #-1
NOT R1, R4
AND R1, R1, R7
STR R1, R0, #0

AND R5, R5, #0 ;R5作为辅助变量

CHANGE AND R5, R3, #1
       BRp TRANS1
       AND R5, R3, #2
       BRp TRANS2
       ADD R5, R4, R2
       BRz FINISH
       BR CHANGE
       
BR FINISH
       
TRANS1 ADD R3, R3, #1
       AND R5, R4, #1
       BRz SUB
       BRp MUL
       
TRANS2 ADD R3, R3, #-1
       AND R5, R5, #0
       ADD R5, R5, #1
       DOUBLE_RET
    LOOP_SE AND R6, R4, R5
            BRz DOUBLE
       ADD R5, R5, R5
       AND R6, R4, R5
       BRz MUL_R5
       BRp SUB_R5
       
       
SUB ADD R5, R4, R2
    BRz FINISH
    ADD R4, R4, #1
    ADD R0, R0, #-1
    NOT R1, R4
    AND R1, R1, R7
    STR R1, R0, #0
    BR CHANGE
    
MUL ADD R5, R4, R2
    BRz FINISH
    ADD R4, R4, #-1
    ADD R0, R0, #-1
    NOT R1, R4
    AND R1, R1, R7
    STR R1, R0, #0
    BR CHANGE

DOUBLE ADD R5, R5, R5
    BR DOUBLE_RET

MUL_R5 ADD R6, R4, R2
       BRz FINISH
       ADD R4, R4, R5
       ADD R0, R0, #-1
       NOT R1, R4
       AND R1, R1, R7
       STR R1, R0, #0
       BR CHANGE

SUB_R5 ADD R6, R4, R2
       BRz FINISH
       NOT R5, R5
       ADD R5, R5, #1
       ADD R4, R4, R5
       ADD R0, R0, #-1
       NOT R1, R4
       AND R1, R1, R7
       STR R1, R0, #0
       BR CHANGE
       
FIRST ADD R3, R3, #1
      ADD R7, R7, R7
      ADD R7, R7, #1
      ADD R4, R4, #-1
      BRz DEBUG
      BR LONG
      
SECOND ADD R3, R3, #-1
       ADD R7, R7, R7
       ADD R4, R4, #-1
       BRz DEBUG
       BR LONG
      
FINISH

LD R0, NUMBER
AND R1, R1, #0
AND R2, R2, #0
ADD R2, R2, #1

COUNT AND R5, R7, R2
      BRz FINISH_END
      ADD R1, R1, #1
      ADD R2, R2, R2
      BR COUNT
      
FINISH_END    

STR R1, R0, #0

HALT

NUMBER .FILL x3100

.END