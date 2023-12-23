.ORIG x3000

;初始化界面
INIT
    AND R6, R6, #0
    ADD R6, R6, #2
    LEA R0, WELCOME_MSG
    PUTS
    LD R1, SUB_W
    GETC
    ADD R0, R0, R1  ;检查输入是否为W
    BRz GET_PASSWORD
    BR RESTART
    
GET_PASSWORD
    LEA R0, PASSWORD_PROMPT   
    PUTS
    LD  R1, PASSWORD_LENGTH  ;R1存储密码长度 PB21020706-10
    LEA R2, USER_PASSWORD  ;R2存储用户输入的密码
    AND R3, R3, #0
    ADD R3, R2, #0  ;R3记录R2的起点位置
    
CHECK_PASSWORD
    GETC
    OUT
    LD R4, SUB_Y
    ADD R4, R0, R4        
    BRz CHECK_SUBMIT
    
    STR R0, R2, #0
    ADD R2, R2, #1
    
    BR  CHECK_PASSWORD
    
CHECK_SUBMIT
    LEA R4, CORRECT_PASSWORD
    LOOP LDR R2, R3, #0
         NOT R2, R2
         ADD R2, R2, #1
         LDR R5, R4, #0
         ADD R5, R2, R5
         BRnp INVALID_SUBMIT
         ADD R1, R1, #-1
         BRz PASSWORD_CORRECT
         ADD R3, R3, #1
         ADD R4, R4, #1
         BR LOOP
         
PASSWORD_CORRECT
    LEA R0, SUCCESS_MSG
    PUTS
    HALT

INVALID_SUBMIT
    LEA R0, INVALID_MSG
    PUTS
    ADD R0, R6, #-2
    BRz OUTPUT_2
    ADD R0, R6, #-1
    BRz OUTPUT_1
    ADD R0, R6, #0
    BRz OUTPUT_0
    OUTPUT_RET
    LEA R0, ATTEMPTS_MSG
    PUTS
    ADD R6, R6, #-1
    BRn FAIL
    BR GET_PASSWORD
    
OUTPUT_2
    LEA R0, OUTPUT_2_MSG 
    PUTS
    BR OUTPUT_RET
    
OUTPUT_1
    LEA R0, OUTPUT_1_MSG 
    PUTS
    BR OUTPUT_RET
    
OUTPUT_0
    LEA R0, OUTPUT_0_MSG 
    PUTS
    BR OUTPUT_RET
    
FAIL
    LEA R0, FAIL_MSG
    PUTS
    BR INIT
    
RESTART
    LEA R0, WELCOME_MSG
    PUTS
    LD R1, SUB_W
    GETC                      
    ADD R0, R0, R1         
    BRz GET_PASSWORD
    BR RESTART

WELCOME_MSG .STRINGZ "Welcome to the bank system! Type 'W' to withdraw some fund.\n"
PASSWORD_PROMPT .STRINGZ "Please input your password: "
INVALID_MSG     .STRINGZ "\nIncorrect password! "
OUTPUT_2_MSG    .STRINGZ "2 "
OUTPUT_1_MSG    .STRINGZ "1 "
OUTPUT_0_MSG    .STRINGZ "0 "
ATTEMPTS_MSG    .STRINGZ "attempt(s) remain.\n"
SUCCESS_MSG     .STRINGZ "\nSuccess!\n"
FAIL_MSG        .STRINGZ "Fails.\n"

SUB_W .FILL #-87
SUB_Y .FILL #-89
TIMES .FILL #48
PASSWORD_LENGTH .FILL #10
USER_PASSWORD .BLKW 10
CORRECT_PASSWORD .STRINGZ "PB21020706"

.END