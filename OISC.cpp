#include <iostream>
#include <fstream>
#include <string>

using std::cout;
using std::flush;

int* REGISTER_FILE = new int[16];

#define r0 REGISTER_FILE[0]
#define r1 REGISTER_FILE[1]
#define r2 REGISTER_FILE[2]
#define r3 REGISTER_FILE[3]
#define r4 REGISTER_FILE[4]
#define r5 REGISTER_FILE[5]
#define r6 REGISTER_FILE[6]
#define r7 REGISTER_FILE[7]
#define r8 REGISTER_FILE[8]
#define r9 REGISTER_FILE[9]
#define r10 REGISTER_FILE[10]
#define r11 REGISTER_FILE[11]
#define r12 REGISTER_FILE[12]
#define r13 REGISTER_FILE[13]
#define r14 REGISTER_FILE[14]
#define r15 REGISTER_FILE[15]

int* LIT1 = new int;
int* LIT2 = new int;
int* LIT3 = new int;

int SBN(int* source, int* target, int* dest, int* jump, int* pc) {
    cout << *dest << " - " << *source;

    *dest = *source - *target; /// subtract
    if(*dest < 0) return *jump; /// branch if negative
    else return *pc + 1;
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

int run(std::string file, int pc) {
    std::ifstream f(file);

    std::string line;
    for(int i=0; i<pc; i++) std::getline(f, line);

    bool end = true;

    while(std::getline(f, line)) {
        int** params = pars(line);
        for(int i=0; i<4; i++) cout << *params[i] << std::endl;
  
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
    std::string file = "file.sbn";
    line_num = run(file, line_num);

    while(line_num){
        run(file, line_num);
    }

    cout << "\n=============================\n";
    cout << "program finished with exit code " << line_num << '\n';
    cout << "\nREGISTER_FILE:\n";
    for(int i=0; i<16; i++) cout << "R" << i << ": " << REGISTER_FILE[i] << "\n";
    cout << flush;

    return 0;
}