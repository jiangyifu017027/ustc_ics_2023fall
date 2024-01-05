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
            // 更新symbol table
            address += 1;
            
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

                }
                else if (tokens[1] == ".STRINGZ") {

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
            address += 1;

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

    }
    if (tokens[0] == "BRZ") {

    }
    if (tokens[0] == "BRP") {

    }
    if (tokens[0] == "BRNZ") {

    }
    if (tokens[0] == "BRNP") {

    }
    if (tokens[0] == "BRZP") {

    }
    if (tokens[0] == "BRNZP") {

    }
    if (tokens[0] == "JMP") {

    }
    if (tokens[0] == "JSR") {

    }
    if (tokens[0] == "JSRR") {

    }
    if (tokens[0] == "LD") {

    }
    if (tokens[0] == "LDI") {

    }
    if (tokens[0] == "LDR") {

    }
    if (tokens[0] == "LEA") {

    }
    if (tokens[0] == "NOT") {

    }
    if (tokens[0] == "RET") {

    }
    if (tokens[0] == "RTI") {

    }
    if (tokens[0] == "ST") {

    }
    if (tokens[0] == "STI") {

    }
    if (tokens[0] == "STR") {

    }
    if (tokens[0] == "TRAP") {

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