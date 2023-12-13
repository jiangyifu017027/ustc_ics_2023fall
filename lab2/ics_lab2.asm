.ORIG x3000

; 从内存中读取N的数值
LD R7, N_ADDRESS
LDR R0, R7, #0  ; R0后续作为计数器
LD R6, MODULO

; 初始化变量
AND R1, R1, #0  ; R1存储v_n的初始值
ADD R1, R1, #3
AND R2, R2, #0  ; R2存储d_n的初始值
ADD R2, R2, #1

LOOP
  ; 判断是否达到N的值
  ADD R0, R0, #-1
  BRz DONE               ; 如果R0 == 0,则跳转到DONE

  ; 计算下一项的值 v_(n+1)
  ADD R1, R1, R2         ; R1 = v_n + d_n
  ADD R1, R1, R1         ; R1 = 2 * (v_n + d_n)
  AND R1, R1, R6         ; mod 4096

  ; 检查 v_(n+1) 是否可以被 8 整除或个位数是否为 8
  ; 检查是否可以被8整除
  ADD R4, R1, #0
  AND R4, R4, #7         ; R4 = v_(n+1) mod 8
  BRz CHANGE
  
  ; 存储R1的值进入R3,R3最后存储个位数字
  AND R3, R3, #0
  ADD R3, R1, #0         
  LD R4, SUB_F
  ; 这里的取模原理参考report
SUB_1
  ADD R3, R3, R4
  BRp SUB_1
  NOT R4, R4
  ADD R4, R4, #1
  ADD R3, R3, R4
  LD R4, SUB_S
SUB_2
  ADD R3, R3, R4
  BRp SUB_2
  NOT R4, R4
  ADD R4, R4, #1
  ADD R3, R3, R4
  LD R4, SUB_T
SUB_3
  ADD R3, R3, R4
  BRp SUB_3
  NOT R4, R4
  ADD R4, R4, #1
  ADD R3, R3, R4
  
  ADD R3, R3, #-8
  BRz CHANGE
  
  BR LOOP
CHANGE
  NOT R2, R2
  ADD R2, R2, #1

  BR LOOP

; 循环结束，存储结果
DONE
  LD R7, RESULT_ADDRESS
  STR R1, R7, #0
  
HALT

N_ADDRESS      .FILL x3102   ; N存储的位置
RESULT_ADDRESS .FILL x3103   ; 存储结果的地址
MODULO         .FILL x0FFF   ; 作为mod4096的辅助
SUB_F          .FILL #-1000  ; 取个位数字辅助
SUB_S          .FILL #-100
SUB_T          .FILL #-10

.END