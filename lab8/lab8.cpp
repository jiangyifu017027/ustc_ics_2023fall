#include <cstdint>
#include <iostream>
#include <fstream>
#include <bitset>

#define LENGTH 1
#define MAXLEN 100
#define STUDENT_ID_LAST_DIGIT 3



int16_t lab1(int16_t n) {
    // initialize
    int result = 0; //记录0的个数

    // calculation
    if ((n & 1) == 0) {
        n = ~n;
        n = n + 1;
    }

    int k = 16;
    int16_t m = 1;

    while (k > 0) {
        k--;
        int16_t temp = n & m;
        if (temp == 0) {
            result++;
        }
        m = m + m;
    }

    result = result + 6; // 加ID尾号 PB21020706

    // return value
    return result;
}

int16_t binTodecimal(int16_t v_n) {
    int16_t sub_f = -1000;
    int16_t sub_s = -100;
    int16_t sub_t = -10;

    while(v_n >= 0) {
        v_n = v_n + sub_f;
    }
    v_n = v_n + 1000;

    while(v_n >= 0) {
        v_n = v_n + sub_s;
    }
    v_n = v_n + 100;
    
    while(v_n >= 0) {
        v_n = v_n + sub_t;
    }
    v_n = v_n + 10;

    return v_n;
}

int16_t lab2(int16_t n) {
    // 从内存中读取N的数值
    int16_t v_n = 3;  // v_n的初始值为3
    int16_t d_n = 1;   // d_n的初始值为1

    // 初始化辅助变量
    int16_t mod = 4095;

    // 循环
    while (n > 1) {
        // 计算下一项的值 v_(n+1)
        v_n = (v_n + d_n + v_n + d_n) & mod;

        // 检查 v_(n+1) 是否可以被 8 整除或个位数是否为 8
        if ((v_n & 7) == 0 || binTodecimal(v_n) == 8) {
            d_n = -d_n;
        }
        n--;
    }

    return v_n;
}

int16_t lab3(char s1[], char s2[]) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

int16_t gray(int16_t result) {
    int16_t temp = 1;
    while ((temp & result) == 0) {
        temp = temp + temp;
    }

    int16_t change = temp + temp;
    if ((result & change) > 0) {
        result = result - change;
    }
    else {
        result = result + change;
    }

    return result;
}

int16_t lab4(int16_t *memory, int16_t n) {
    // initialize
    int16_t R2 = 0;
    int16_t num = 1;
    int16_t m = n;

    while (m > 0) {
        R2 = R2 + num;
        num = num + num;
        m--;
    }

    int16_t R7 = R2;

    R2 = ~R2;
    R2 = R2 + 1;

    int16_t k = 0;
    int16_t step = 0; // step是完成步数

    if ((n & 1) == 0) {
        int16_t m = n;
        int16_t temp = 2;
        while (m > 0) {
            step = step + temp;
            temp = temp + temp;
            temp = temp + temp;
            m = m - 2;
        }
    }
    else {
        int16_t m = n;
        int16_t temp = 1;
        while (m > 0) {
            step = step + temp;
            temp = temp + temp;
            temp = temp + temp;
            m = m - 2;
        }
    }

    int odd_point = 1;
    int16_t result = 0;

    for (int i = step - 1; i >= 0 ; --i) {
        int16_t val = ~result;
        val = val & R7;
        memory[i] = val;

        if (odd_point == 1) {
            odd_point = odd_point + 1;
            if ((result & 1) == 0) {
                result = result + 1;
            }
            else {
                result = result - 1;
            }
        }
        else {
            odd_point = odd_point - 1;
            result = gray(result);
        }
    }

    return step;
}

int main() {
    std::fstream file;
    file.open("test.txt", std::ios::in);

    // lab1
    int16_t n = 0;
    std::cout << "===== lab1 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab1(n) << std::endl;
    }

    // lab2
    std::cout << "===== lab2 =====" << std::endl;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        std::cout << lab2(n) << std::endl;
    }

    // lab3
    std::cout << "===== lab3 =====" << std::endl;
    char s1[MAXLEN]; char s2[MAXLEN];
    for (int i = 0; i < LENGTH; ++i) {
        file >> s1 >> s2;
        std::cout << lab3(s1, s2) << std::endl;
    }
    
    // lab4
    std::cout << "===== lab4 =====" << std::endl;
    int16_t memory[MAXLEN], move;
    for (int i = 0; i < LENGTH; ++i) {
        file >> n;
        int16_t state = 0;
        move = lab4(memory, n);
        for(int j = 0; j < move; ++j){
            std::cout << std::bitset<16>(memory[j]) << std::endl;
        }
    }
    
    file.close();
    return 0;
}