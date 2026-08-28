#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <cstdio>
#include <unistd.h>

// Register file
int* REGISTER_FILE = new int[16];

// Literal values
int* LIT1 = new int;
int* LIT2 = new int;
int* LIT3 = new int;

std::map <std::string, int> LABELS;


int SBN(int* source, int* target, int* dest, int* jump, int* pc) {
    *dest = *source - *target; /// subtract
    if(*dest < 0) return *jump; /// branch if negative
    else return *pc + 1;
}

void output(const std::string& line) {
    // executes an output line in the format:
    /// out r12
    /// out hello

    std::string param;
    for(int i=3; i<line.size(); i++) {
        if(line[i] == ' ') continue;
        param.push_back(line[i]);
    }

    if(param.empty()) return;

    if(param[0] == 'r') {
        int reg = stoi(param.substr(1));
        std::cout << REGISTER_FILE[reg] << std::endl;
    } else {
        if(param[0] == '\\') param = param.substr(1);
        std::cout << param << std::endl;
    }
}

void link(const std::string& line, int line_number) {
    // executes a link line like:
    /// link r5

    // it will store the next line number in the given register

    int reg;
    for(int i=0; i<line.size(); i++)
        if(line[i] == 'r') {
            reg = stoi(line.substr(i+1));
            break;
        }

    REGISTER_FILE[reg] = line_number + 1;
}

void input(const std::string& line) {
    // executes an input line in the format:
    /// inp r12

    std::string param;
    for(int i=3; i<line.size(); i++) {
        if(line[i] == ' ') continue;
        param.push_back(line[i]);
    }

    if(param.empty() || param[0] != 'r') return;
    param = param.substr(1);

    int reg = stoi(param);
    std::cin >> REGISTER_FILE[reg];
}

bool is_numeric(const char c)   {return '0' <= c && c <= '9';}
bool is_lowercase(const char c) {return 'a' <= c && c <= 'z';}
bool is_uppercase(const char c) {return 'A' <= c && c <= 'Z';}
bool is_letter(const char c) {return is_lowercase(c) || is_uppercase(c);}
std::string pre_compile(std::string file)
{
    /// parses the whole file and finds the labels' names to line numbers
    /// Label names should not start with numbers or '+' of '-'
    /// Label names should only contain letters and numbers

    std::ifstream f(file);
    std::string line;
    int line_number = 1;
    while(std::getline(f, line)) {
        if(line.size() && line[0] == 'L') {
            int i=1;
            while(i<line.size() && line[i]==' ') i++;

            std::string label;
            for(i; i<line.size(); i++) {
                if(is_numeric(line[i]) || is_letter(line[i])) label.push_back(line[i]);
                else return "label contains invalid character at line " + std::to_string(line_number);
            }

            if(label.empty() || is_numeric(label[0]) || label[0] == '+' || label[0] == '-')
                return "label is empty or starts with an invalid character at line " + std::to_string(line_number);
            
            if(LABELS.find(label) != LABELS.end())
                return "label is declared twice at line " + std::to_string(line_number);
            LABELS.insert({label, line_number});
        }
        
        line_number ++;
    }
    return "ok";
}

int** pars(std::string line, int current_pc) {
    /// Parses an sbn line and finds all the arguments
    /// Returns an array of pointers to arguments values

    for(auto x : line) if(is_uppercase(x)) x -= 'A' - 'a'; /// to lowercase
    while(line.back() == ' ') line.pop_back();
    line.push_back(',');

    int** params = new int*[4];
    int i = 0;
    std::string current;
    for(auto x : line)
    {
        if(x == ' ') continue;
        else if(x == ','){
            if(current.empty()) {
                std::cout << "parser: missing argument in line ";
                return nullptr;
            }

            std::string num;
            int reg;
            switch (i)
            {
            case 1:
                if(current[0] == 'r')
                {
                    num = current.substr(1);
                    reg = std::stoi(num);
                    params[i-1] = REGISTER_FILE + reg;
                }
                else
                {
                    *LIT1 = std::stoi(current);
                    params[i-1] = LIT1;
                }
                break;

            case 2:
                if(current[0] == 'r')
                {
                    num = current.substr(1);
                    reg = std::stoi(num);
                    params[i-1] = REGISTER_FILE + reg;
                }
                else
                {
                    *LIT2 = std::stoi(current);
                    params[i-1] = LIT2;
                }
                break;

            case 3:
                num = current.substr(1);
                reg = std::stoi(num);
                params[i-1] = REGISTER_FILE + reg;
                break;

            case 4:
                if(current[0] == 'r') {
                    int reg = stoi(current.substr(1));
                    *LIT3 = REGISTER_FILE[reg];
                } else if(current[0] == '+' || current[0] == '-') {
                    *LIT3 = std::stoi(current.substr(1)) + current_pc;
                } else if(is_numeric(current[0])) {
                    *LIT3 = std::stoi(current);
                } else {
                    *LIT3 = LABELS[current];
                }
                params[i-1] = LIT3;
                break;
            
            default:
                break;
            }

            i++;
            current.clear();
        }

        else {
            current.push_back(x);
        }
    }
    
    return params;
}

int run_block(std::string file, int pc) {
    std::ifstream f(file);

    std::string line;
    for(int i=1; i<pc; i++) std::getline(f, line);

    bool end = true;

    while(std::getline(f, line)) {
        if(line.empty()) continue;
        if(line[0] == '#' || line[0] == '\n' || line[0]=='L') continue;

        ///input
        if(line.size()>3 && line.substr(0, 3) == "inp") {
            input(line);
            pc ++;
            continue;
        }

        ///output
        if(line.size()>3 && line.substr(0, 3) == "out") {
            output(line);
            pc ++;
            continue;
        }

        ///link
        if(line.size()>4 && line.substr(0, 4) == "link") {
            link(line, pc);
            pc ++;
            continue;
        }

        int** params = pars(line, pc);
        if(!params) {
            std::cout << "error at line " << pc << std::endl;
            return -1;
        }
  
        int new_pc = SBN(params[0], params[1], params[2], params[3], &pc);
        delete []params;

        if(new_pc != pc + 1){
            /// branch occuered
            pc = new_pc; /// pc should be updated
            end = false; /// that is not the end of program for sure
            break; /// this while loop should break and a new one from the new_pc should start
        }
        pc = new_pc;
    }

    if(end) return 0;
    else return pc;
}

int run(std::string file) {
    //check if file exists
    std::ifstream file_exist(file.c_str());
    bool exist = file_exist.good();
    file_exist.close();
    if(!exist) return -1;

    std::string pre_compile_result = pre_compile(file);
    if(pre_compile_result != "ok")
    {
        std::cout << "precompile error: " << pre_compile_result << std::endl;
        return -1;
    }

    int line_num = 1;
    while(line_num>0){
        line_num = run_block(file, line_num);
    }
    return line_num;
}

void RF_log() {
    std::cout << "\n=============================\n";
    std::cout << "REGISTER_FILE:\n";
    for(int i=0; i<16; i++) std::cout << "R" << i << ": " << REGISTER_FILE[i] << "\n";
    std::cout << std::flush;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "one argument needed! (file name)\n" << std::flush;
        return 0;
    }

    bool debug = getopt(argc, argv, "d") != -1;

    std::string file = argv[1];
    int ex = run(file);
    std::cout << "\nprogram finished with exit code " << ex << '\n' << std::flush;
    if(debug) RF_log();

    return 0;
}