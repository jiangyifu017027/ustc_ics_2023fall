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
std::string translate_instruction(const std::string &instruction);

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

const std::unordered_set<std::string> instruction = {
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

const std::unordered_set<std::string> pseudo_op = {
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
            
            std::string newToken = tokens[0];
            if (instruction.count(newToken) <= 0 && pseudo_op.count(newToken) <= 0) {
                symbolTable[newToken] = address;
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
            }
            else {

            }

        }

    }

    return output_lines;
} 

const std::unordered_map<std::string, std::string> register = {
    {"R0", "000"},
    {"R1", "001"},
    {"R2", "010"},
    {"R3", "011"},
    {"R4", "100"},
    {"R5", "101"},
    {"R6", "110"},
    {"R7", "111"}
};

std::string translate_instruction(const std::vector<std::string> &tokens, int &address, const std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    // TODO: Implement the translation of an individual instruction
    //       to machine code.
    if (tokens[0] == "ADD") {
        machine_code = ADD_handle(tokens, address, symbolTable);
    }
    else if (tokens[0] == "AND") {

    }
    else if (tokens[0] == "BR") {

    }
    else if (tokens[0] == "BRN") {

    }
    else if (tokens[0] == "BRZ") {

    }
    else if (tokens[0] == "BRP") {

    }
    else if (tokens[0] == "BRNZ") {

    }
    else if (tokens[0] == "BRNP") {

    }
    else if (tokens[0] == "BRZP") {

    }
    else if (tokens[0] == "BRNZP") {

    }
    else if (tokens[0] == "JMP") {

    }
    else if (tokens[0] == "JSR") {

    }
    else if (tokens[0] == "JSRR") {

    }
    else if (tokens[0] == "LD") {

    }
    else if (tokens[0] == "LDI") {

    }
    else if (tokens[0] == "LDR") {

    }
    else if (tokens[0] == "LEA") {

    }
    else if (tokens[0] == "NOT") {

    }
    else if (tokens[0] == "RET") {

    }
    else if (tokens[0] == "RTI") {

    }
    else if (tokens[0] == "ST") {

    }
    else if (tokens[0] == "STI") {

    }
    else if (tokens[0] == "STR") {

    }
    else if (tokens[0] == "TRAP") {

    }

    return machine_code;
}

std::string ADD_handle(const std::vector<std::string> &tokens, int &address, const std::unordered_map<std::string, int> &symbolTable) {
    std::string machine_code;

    machine_code += "0001";

    for (size_t i = 1; i < tokens.size(); ++i) {
        switch (i):
            case 1:
                machine_code += register[tokens[i]];
                break;
            case 2:
                machine_code += register[tokens[i]];
                break;
            case 3:
                if (register.count(tokens[i]) != 0) {
                    machine_code += "000";
                    machine_code += register[tokens[i]];
                }
                else {
                    machine_code += "1";
                    std::string str = tokens[i].substr(0, 1);
                    std::string number = tokens[i].substr(1);
                    
                }
    }
}