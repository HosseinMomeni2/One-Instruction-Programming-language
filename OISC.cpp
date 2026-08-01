#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <map>

using std::cout;
using std::flush;

// Register file
int* REGISTER_FILE = new int[16];

// Literal values
int* LIT1 = new int;
int* LIT2 = new int;
int* LIT3 = new int;


int SBN(int* source, int* target, int* dest, int* jump, int* pc) {
    // cout << *dest << " - " << *source;

    *dest = *source - *target; /// subtract
    if(*dest < 0) return *jump; /// branch if negative
    else return *pc + 1;
}

std::string pre_compile(std::string file, std::string tar = ".temp.OISC")
{
    /// parses the whole file and converts the labels' names to line number

    // Labels
    std::map<std::string, int> LABELS;

    std::ifstream f(file);
    std::ofstream t(tar);
    std::string line;
    int line_number = 0;
    while(std::getline(f, line)) {
        if(line[0] == '#') continue;

        if(line[0] == 'L') {
            std::string label = line.substr(2);
            LABELS[label] = line_number + 1;
            t << "###" << label << '\n';

        } else {
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

            t << line << '\n';
        }

        line_number ++;
    }

    f.close();
    t.close();

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
        if(line[0] == '#') continue;

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

int main() {
    int line_num = 1;
    pre_compile("file.sbn", "file.txt");
    std::string file = "file.txt";
    line_num = run_block(file, line_num);

    while(line_num){
        run_block(file, line_num);
    }

    cout << "\n=============================\n";
    cout << "program finished with exit code " << line_num << '\n';
    cout << "\nREGISTER_FILE:\n";
    for(int i=0; i<16; i++) cout << "R" << i << ": " << REGISTER_FILE[i] << "\n";
    cout << flush;

    return 0;
}