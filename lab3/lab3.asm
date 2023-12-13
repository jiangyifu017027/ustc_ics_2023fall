.ORIG x3000
; Begin of your code

LD R0 S1_ADDR
LD R1 S2_ADDR

LOOP LDR R3, R0, #0
     LDR R4, R1, #0
     NOT R4, R4
     ADD R4, R4, #1
     ADD R2, R3, R4
     
     BRnp STORE
     ADD R3, R3, #0
     BRz STORE
     ADD R0, R0, #1
     ADD R1, R1, #1
     BR LOOP

STORE
; End of your code
STI R2, RESULT ; write back you result
HALT

S1_ADDR .FILL x3100
S2_ADDR .FILL x3200
RESULT .FILL x3300
.END