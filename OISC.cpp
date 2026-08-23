#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <cstdio>

// Register file
int* REGISTER_FILE = new int[16];

// Literal values
int* LIT1 = new int;
int* LIT2 = new int;
int* LIT3 = new int;


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

std::string pre_compile(std::string file, std::string tar = ".temp.OISC")
{
    /// parses the whole file and converts the labels' names to line number

    // Labels
    std::map<std::string, int> LABELS;

    std::ifstream f(file);
    std::ofstream temp_write(tar+".temp");
    std::string line;
    int line_number = 0;
    while(std::getline(f, line)) {
        if(line.empty()) continue;
        if(line[0] == '#' || line[0] == '\n') continue;

        if(line[0] == 'L') {
            std::string label = line.substr(2);
            LABELS[label] = line_number;
            temp_write << "###" << label << '\n';

        } else {
            temp_write << line << '\n';
        }

        line_number ++;
    }
    temp_write.close();
    f.close();

    std::ofstream target(tar);
    std::ifstream temp_read(tar + ".temp");
    while(getline(temp_read, line)) {
        if(line.size() < 3) continue;
        if(line.substr(0, 3) == "sbn")
        {
            std::string label;
            int comma = 0;
            for(auto x : line) {
                if(x == '#') break;

                if(x == ',') comma++;
                else if(x != ' ' && comma == 4) {
                    label.push_back(x);
                }
            }
            if(label.empty()) label = "###";

            while(line.back() != ',') line.pop_back();
            line.push_back(' ');
            line += std::to_string(LABELS[label]);
        }

        target << line << '\n';
    }

    target.close();
    temp_read.close();
    remove((tar + ".temp").c_str());

    return tar;
}

int** pars(std::string line) {
    for(auto x : line) if(x>='A' && x<='Z') x -= 'A' - 'a'; /// to lowercase
    while(line.back() == ' ') line.pop_back();
    line.push_back(',');

    int** params = new int*[4];
    int i = 0;
    std::string current;
    for(auto x : line)
    {
        if(x == ' ') continue;
        else if(x == ','){
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
                *LIT3 = std::stoi(current);
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
    for(int i=0; i<pc; i++) std::getline(f, line);

    bool end = true;

    while(std::getline(f, line)) {
        if(line.empty()) continue;
        if(line[0] == '#' || line[0] == '\n') continue;

        if(line.size()>3 && line.substr(0, 3) == "out")
        {
            output(line);
            continue;
        }

        int** params = pars(line);
  
        int new_pc = SBN(params[0], params[1], params[2], params[3], &pc);
        delete []params;

        if(new_pc != pc + 1){
            pc = new_pc;
            end = false;
            break;
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

    int line_num = 1;
    file = pre_compile(file, "file.txt");
    line_num = run_block(file, line_num);

    while(line_num){
        line_num = run_block(file, line_num);
    }

    remove(file.c_str());
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

    bool debug = false;

    std::string file = argv[1];
    int ex = run(file);
    std::cout << "\nprogram finished with exit code " << ex << '\n' << std::flush;
    if(debug) RF_log();

    return 0;
}