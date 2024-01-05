#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <bitset>

std::vector<std::string> read_asm_file(const std::string &filename);
void write_output_file(const std::string &filename, const std::vector<std::string> &output);
std::vector<std::string> assemble(const std::vector<std::string> &input_lines);
std::string translate_instruction(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string ADD_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string AND_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRN_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRZ_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRNZ_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRZP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRNP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string BRNZP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string JMP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string RET_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string JSR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string JSRR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string LD_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string LDI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string LDR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string LEA_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string NOT_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string RTI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string ST_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string STI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string STR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);
std::string TRAP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable);


// TODO: Define any additional functions you need to implement the assembler, e.g. the symbol table.

int main(int argc, char *argv[]) {
    // Command-line argument parsing
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file.asm> <output_file.txt>" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    // Read the input ASM file
    std::vector<std::string> input_lines = read_asm_file(input_filename);

    // Assemble the input file
    std::vector<std::string> output_lines = assemble(input_lines);

    // Write the output file
    write_output_file(output_filename, output_lines);

    return 0;
}

std::vector<std::string> read_asm_file(const std::string &filename) {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream file(filename);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }

    return lines;
}

void write_output_file(const std::string &filename, const std::vector<std::string> &output) {
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &line : output)
        {
            file << line << std::endl;
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

std::unordered_set<std::string> instruction = {
    "ADD",
    "AND",
    "BR",
    "BRN",
    "BRZ",
    "BRP",
    "BRNZ",
    "BRZP",
    "BRNZP",
    "JMP",
    "JSR",
    "JSRR",
    "LD",
    "LDI",
    "LDR",
    "LEA",
    "NOT",
    "RET",
    "RTI",
    "ST",
    "STI",
    "STR",
    "TRAP"
};

std::unordered_set<std::string> pseudo_op = {
    ".FILL",
    ".STRINGZ",
    ".BLKW",
    ".END"
};

// 分割字符串的辅助函数
std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = input.find(delimiter);
    
    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }
    
    tokens.push_back(input.substr(start));
    return tokens;
}

// 十进制数变为二进制字符串
std::string decimalToBinary(int decimal) {
    std::bitset<16> binary(decimal);
    std::string binaryString = binary.to_string();
    return binaryString;
}

std::string charToBinary(char &c) {
    std::bitset<16> binary(static_cast<unsigned char>(c)); // 将字符转换为无符号字符，并使用 std::bitset 转换为二进制形式的字符串
    std::string binaryString = binary.to_string(); // 将 std::bitset 转换为 std::string

    return binaryString;
}

std::vector<std::string> assemble(const std::vector<std::string> &input_lines) {
    std::vector<std::string> output_lines;
    int address = 0;
    std::unordered_map<std::string, int> symbolTable;

    // TODO: Implement the assembly process
    // Implement the 2-pass process described in textbook.

    // the first pass
    for (size_t i = 0; i < input_lines.size(); ++i) {
        std::string line = input_lines[i];
        // 分割字符串
        std::vector<std::string> tokens = splitString(line, ' ');

        // 去掉','
        for (size_t j = 0; j < tokens.size(); ++j) {
            size_t length = tokens[j].size() - 1;
            if (tokens[j][length] == ',') {
                tokens[j].pop_back();
            }
        }

        if (tokens[0] == ".ORIG") {
            std::string newToken = tokens[1].substr(1);
            address = std::stoi(newToken, nullptr, 16);
            std::string machine_code = decimalToBinary(address);
            output_lines.push_back(machine_code);
            address -= 1;
            continue;
        }
        else {
            if (tokens[0] == ".END") {
                break;
            }

            // 断点测试
            // std::cout << "Hello!" << std::endl;

            // 更新symbol table
            address += 1;

            // 防止后面vector越界
            if (tokens[0] == "RET") {
                continue;
            }
            
            if (instruction.count(tokens[0]) <= 0 && pseudo_op.count(tokens[1]) <= 0) {
                symbolTable[tokens[0]] = address;
            }
            else if (pseudo_op.count(tokens[1]) >= 1) {
                if (tokens[1] == ".FILL") {
                    std::string FILL = tokens[2];
                    std::string subFILL = FILL.substr(1, 1);
                    std::string number = FILL.substr(1);
                    
                    if (subFILL == "x") {
                        symbolTable[tokens[0]] = std::stoi(number, nullptr, 16);
                    }
                    else if (subFILL == "#") {
                        symbolTable[tokens[0]] = std::stoi(number, nullptr, 10);
                    }
                }
                else if (tokens[1] == ".BLKW") {
                    symbolTable[tokens[0]] = address;
                    std::string BLKW = tokens[2];
                    std::string subBLKW = BLKW.substr(1, 1);
                    std::string number = BLKW.substr(1);

                    if (subBLKW == "x") {
                        int temp = std::stoi(number, nullptr, 16) - 1;
                        address += temp;
                    }
                    else if (subBLKW == "#") {
                        int temp = std::stoi(number, nullptr, 10) - 1;
                        address += temp;
                    }
                }
                else if (tokens[1] == ".STRINGZ") {
                    symbolTable[tokens[0]] = address;
                    int length = tokens[2].size();
                    std::string str = tokens[2].substr(1, length - 2);

                    // STRINGZ存入内存应该在the second pass中实现

                    address += str.size();
                }
            }
        }
    }

    // the second pass
    for (size_t i = 0; i < input_lines.size(); ++i) {
        std::string line = input_lines[i];
        std::vector<std::string> tokens = splitString(line, ' ');

        for (size_t j = 0; j < tokens.size(); ++j) {
            size_t length = tokens[j].size() - 1;
            if (tokens[j][length] == ',') {
                tokens[j].pop_back();
            }
        }

        if (tokens[0] == ".ORIG") {
            std::string newToken = tokens[1].substr(1);
            address = std::stoi(newToken, nullptr, 16);
            address -= 1;
            continue;
        }
        else {
            if (tokens[0] == ".END") {
                break;
            }

            address += 1;

            // 断点测试
            // std::cout << "string!" << std::endl;

            if (instruction.count(tokens[0]) > 0) {
                std::string machine_code = translate_instruction(tokens, address, symbolTable);
                output_lines.push_back(machine_code);
            }
            else {
                if (instruction.count(tokens[1]) > 0) {
                    tokens.erase(tokens.begin());
                    std::string machine_code = translate_instruction(tokens, address, symbolTable);
                    output_lines.push_back(machine_code);
                }
                else {
                    // 考虑pseudo_op情况
                    tokens.erase(tokens.begin());
                    if (tokens[0] == ".FILL") {
                        std::string Label = tokens[1];
                        std::string subLabel = Label.substr(0, 1);
                        std::string number = Label.substr(1);

                        if (subLabel == "x") {
                            int decimal = std::stoi(number, nullptr, 16);
                            std::string result = decimalToBinary(decimal);
                            output_lines.push_back(result);
                        }
                        else if (tokens[0] == "#") {
                            int decimal = std::stoi(number, nullptr, 10);
                            std::string result = decimalToBinary(decimal);
                            output_lines.push_back(result);
                        }
                    }
                    else if (tokens[0] == ".BLKW") {
                        std::string Label = tokens[1];
                        std::string subLabel = Label.substr(0, 1);
                        std::string number = Label.substr(1);

                        int decimal = 0;
                        if (subLabel == "x") {
                            decimal = std::stoi(number, nullptr, 16);
                        }
                        else if (subLabel == "#") {
                            decimal = std::stoi(number, nullptr, 10);
                        }

                        std::string result = "0000000000000000";
                        for (int i = 0; i < decimal; ++i) {
                            output_lines.push_back(result);
                        }
                        address += decimal - 1;
                    }
                    else if (tokens[0] == ".STRINGZ") {
                        std::string Label = tokens[1];
                        int length = Label.size();
                        std::string str = Label.substr(1, length - 2);

                        for (auto ch : str) {
                            std::string binaryCh = charToBinary(ch);
                            output_lines.push_back(binaryCh);
                        }
                        std::string result = "0000000000000000";
                        output_lines.push_back(result);

                        address += str.size();
                    }
                }
            }

        }

    }

    return output_lines;
} 

std::unordered_map<std::string, std::string> Register = {
    {"R0", "000"},
    {"R1", "001"},
    {"R2", "010"},
    {"R3", "011"},
    {"R4", "100"},
    {"R5", "101"},
    {"R6", "110"},
    {"R7", "111"}
};

std::string translate_instruction(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    // TODO: Implement the translation of an individual instruction
    //       to machine code.
    if (tokens[0] == "ADD") {
        machine_code = ADD_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "AND") {
        machine_code = AND_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BR") {
        machine_code = BR_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRN") {
        machine_code = BRN_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRZ") {
        machine_code = BRZ_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRP") {
        machine_code = BRP_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRNZ") {
        machine_code = BRNZ_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRNP") {
        machine_code = BRNP_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRZP") {
        machine_code = BRZP_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "BRNZP") {
        machine_code = BRNZP_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "JMP") {
        machine_code = JMP_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "JSR") {
        machine_code = JSR_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "JSRR") {
        machine_code = JSRR_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "LD") {
        machine_code = LD_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "LDI") {
        machine_code = LDI_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "LDR") {
        machine_code = LDR_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "LEA") {
        machine_code = LEA_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "NOT") {
        machine_code = NOT_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "RET") {
        machine_code = RET_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "RTI") {
        machine_code = RTI_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "ST") {
        machine_code = ST_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "STI") {
        machine_code = STI_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "STR") {
        machine_code = STR_handle(tokens, address, symbolTable);
    }
    if (tokens[0] == "TRAP") {
        machine_code = TRAP_handle(tokens, address, symbolTable);
    }

    return machine_code;
}

std::string decimalToBinary_imm(int decimal) {
    // 处理负数的情况
    if (decimal < 0) {
        // 将负数转换为补码
        decimal = -decimal;
        decimal = (~decimal) + 1;
    }

    // 将十进制数转换为二进制字符串
    std::string binary = std::bitset<32>(decimal).to_string();

    // 截取后5位二进制数
    std::string result = binary.substr(27, 5);

    return result;
}

std::string ADD_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0001";

    for (size_t i = 1; i < tokens.size(); ++i) {
        switch (i) {
            case 1:
                machine_code += Register[tokens[i]];
                break;
            case 2:
                machine_code += Register[tokens[i]];
                break;
            case 3:
                if (Register.count(tokens[i]) != 0) {
                    machine_code += "000";
                    machine_code += Register[tokens[i]];
                }
                else {
                    machine_code += "1";
                    std::string str = tokens[i].substr(0, 1);
                    std::string number = tokens[i].substr(1);
                    int decimal = 0;
                    if (str == "x") {
                        decimal = std::stoi(number, nullptr, 16);
                    }
                    else if (str == "#") {
                        decimal = std::stoi(number, nullptr, 10);
                    }
                    std::string imm = decimalToBinary_imm(decimal);
                    machine_code += imm;
                }
        }
    }

    return machine_code;
}

// AND的实现逻辑和ADD基本一致
std::string AND_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0101";

    for (size_t i = 1; i < tokens.size(); ++i) {
        switch (i) {
            case 1:
                machine_code += Register[tokens[i]];
                break;
            case 2:
                machine_code += Register[tokens[i]];
                break;
            case 3:
                if (Register.count(tokens[i]) != 0) {
                    machine_code += "000";
                    machine_code += Register[tokens[i]];
                }
                else {
                    machine_code += "1";
                    std::string str = tokens[i].substr(0, 1);
                    std::string number = tokens[i].substr(1);
                    int decimal = 0;
                    if (str == "x") {
                        decimal = std::stoi(number, nullptr, 16);
                    }
                    else if (str == "#") {
                        decimal = std::stoi(number, nullptr, 10);
                    }
                    std::string imm = decimalToBinary_imm(decimal);
                    machine_code += imm;
                }
        }
    }

    return machine_code;
}

std::string decimalToBinary_PCoffset9(int decimal) {
    // 处理负数的情况
    if (decimal < 0) {
        // 将负数转换为补码
        decimal = -decimal;
        decimal = (~decimal) + 1;
    }

    // 将十进制数转换为二进制字符串
    std::string binary = std::bitset<32>(decimal).to_string();

    // 截取后9位二进制数
    std::string result = binary.substr(23, 9);

    return result;
}

std::string BR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "111";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRN_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "100";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRZ_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "010";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "001";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRNZ_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "110";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRNP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "101";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRZP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "011";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string BRNZP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0000";
    machine_code += "111";

    std::string Label = tokens[1];
    int PCoffset_9 = symbolTable[Label] - address - 1;

    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string JMP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1100";
    machine_code += "000";
    machine_code += Register[tokens[1]];
    machine_code += "000000";

    return machine_code;
}

std::string RET_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1100";
    machine_code += "000";
    machine_code += "111";
    machine_code += "000000";

    return machine_code;
}

std::string decimalToBinary_PCoffset11(int decimal) {
    // 处理负数的情况
    if (decimal < 0) {
        // 将负数转换为补码
        decimal = -decimal;
        decimal = (~decimal) + 1;
    }

    // 将十进制数转换为二进制字符串
    std::string binary = std::bitset<32>(decimal).to_string();

    // 截取后9位二进制数
    std::string result = binary.substr(21, 11);

    return result;
}

std::string JSR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0100";
    machine_code += "1";

    std::string Label = tokens[1];
    int PCoffset_11 = symbolTable[Label] - address - 1;

    std::string strPCoffset_11 = decimalToBinary_PCoffset11(PCoffset_11);
    machine_code += strPCoffset_11;

    return machine_code;
}

std::string JSRR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0100";
    machine_code += "0";
    machine_code += "00";
    machine_code += Register[tokens[1]];
    machine_code += "000000";

    return machine_code;
}

std::string LD_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0010";
    machine_code += Register[tokens[1]];

    std::string Label = tokens[2];
    int PCoffset_9 = symbolTable[Label] - address - 1;
    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string LDI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1010";
    machine_code += Register[tokens[1]];

    std::string Label = tokens[2];
    int PCoffset_9 = symbolTable[Label] - address - 1;
    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string decimalToBinary_PCoffset6(int decimal) {
    // 处理负数的情况
    if (decimal < 0) {
        // 将负数转换为补码
        decimal = -decimal;
        decimal = (~decimal) + 1;
    }

    // 将十进制数转换为二进制字符串
    std::string binary = std::bitset<32>(decimal).to_string();

    // 截取后6位二进制数
    std::string result = binary.substr(26, 6);

    return result;
}

std::string LDR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0110";
    machine_code += Register[tokens[1]];
    machine_code += Register[tokens[2]];

    std::string PCoffset = tokens[3];
    std::string subPC = PCoffset.substr(0, 1);
    std::string number = PCoffset.substr(1);

    if (subPC == "x") {
        int decimal = std::stoi(number, nullptr, 16);
        machine_code += decimalToBinary_PCoffset6(decimal);
    }
    else if (subPC == "#") {
        int decimal = std::stoi(number, nullptr, 10);
        machine_code += decimalToBinary_PCoffset6(decimal);
    }

    return machine_code;
}

std::string LEA_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1110";
    machine_code += Register[tokens[1]];

    std::string Label = tokens[2];
    int PCoffset_9 = symbolTable[Label] - address - 1;
    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string NOT_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1001";
    machine_code += Register[tokens[1]];
    machine_code += Register[tokens[2]];
    machine_code += "1";
    machine_code += "11111";

    return machine_code;
}

std::string RTI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1000";
    machine_code += "000000000000";

    return machine_code;
}

std::string ST_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0011";
    machine_code += Register[tokens[1]];

    std::string Label = tokens[2];
    int PCoffset_9 = symbolTable[Label] - address - 1;
    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string STI_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1011";
    machine_code += Register[tokens[1]];

    std::string Label = tokens[2];
    int PCoffset_9 = symbolTable[Label] - address - 1;
    std::string strPCoffset_9 = decimalToBinary_PCoffset9(PCoffset_9);
    machine_code += strPCoffset_9;

    return machine_code;
}

std::string STR_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0111";
    machine_code += Register[tokens[1]];
    machine_code += Register[tokens[2]];

    std::string PCoffset = tokens[3];
    std::string subPC = PCoffset.substr(0, 1);
    std::string number = PCoffset.substr(1);

    if (subPC == "x") {
        int decimal = std::stoi(number, nullptr, 16);
        machine_code += decimalToBinary_PCoffset6(decimal);
    }
    else if (subPC == "#") {
        int decimal = std::stoi(number, nullptr, 10);
        machine_code += decimalToBinary_PCoffset6(decimal);
    }

    return machine_code;
}

std::string decimalToBinary_PCoffset8(int decimal) {
    // 处理负数的情况
    if (decimal < 0) {
        // 将负数转换为补码
        decimal = -decimal;
        decimal = (~decimal) + 1;
    }

    // 将十进制数转换为二进制字符串
    std::string binary = std::bitset<32>(decimal).to_string();

    // 截取后8位二进制数
    std::string result = binary.substr(24, 8);

    return result;
}

std::string TRAP_handle(const std::vector<std::string> &tokens, int &address, std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "1111";
    machine_code += "0000";

    std::string trapvect = tokens[1].substr(1);
    int decimal = std::stoi(trapvect, nullptr, 16);

    std::string Trapvect = decimalToBinary_PCoffset8(decimal);
    machine_code += Trapvect;

    return machine_code;
}
