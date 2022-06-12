#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

/* https://www.codingame.com/training/medium/mcxxxx-microcontroller-simulation
**/
using namespace std;

const int INTR_COUNT = 12;
const string INSTRUTIONS[] = {"mov", "jmp",         // basic
    "add", "sub", "mul", "not", "dgt", "dst", // math
    "teq", "tgt", "tlt", "tcp"};              // branching
int acc = 0, dat = 0; // data registers
int inpInd = 0; //input index

int findInstruction(string command);
void getArgs(string* args,string command);
void getLables(vector<string>* code, unordered_map<string,int>* lables, int nComs);
int* getArgValues(string* args,int* inputs);
void mov(string* args, int* inputs);
void add(string* args, int* inputs);
void sub(string* args, int* inputs);
void mul(string* args, int* inputs);
void notAcc();
void dgt (string* args, int* inputs);
void dst (string* args, int* inputs);
void teq(string* args,int* inputs, int* cState);
void tgt(string* args,int* inputs, int* cState);
void tlt(string* args,int* inputs, int* cState);
void tcp(string* args,int* inputs, int* cState);

int main()
{
    int numInputs;
    cin >> numInputs; cin.ignore();
    int* inputs = new int[numInputs];
    for (int i = 0; i < numInputs; i++) {
        cin >> inputs[i]; cin.ignore();
    }
    int numCommands;
    cin >> numCommands; cin.ignore();
    vector<string> code;
    for (int i = 0; i < numCommands; i++) {
        string tmp;
        getline(cin, tmp);
        code.push_back(tmp);
    }
    
    int condState = 0; // conditional state (none,s+,-)
    unordered_map<string, int> lables; // contain lables and line number of each lable
    getLables(&code, &lables, numCommands);
    for (int c = 0; c < numCommands; c++) {
        int cId = findInstruction(code[c]);
        string* args = new string[2];
        acc = clamp(acc,-999,999);
        dat = clamp(dat,-999,999);

        // cerr << code[c] << " ,line: " << c <<endl;
        if (code[c][0] == '#') {
            delete[] args;
            continue;
        }
        else if (code[c][0] == '@') {
            string tmp = code[c];
            code.erase(code.begin() + c);
            tmp = tmp.substr(2,tmp.length()-1);
            c--;
            numCommands--;
            getArgs(args,tmp);
        }
        else if (code[c][0] == '+') {
            if (condState == 1) {
                string tmp = code[c];
                tmp = tmp.substr(2,tmp.length()-1);
                getArgs(args, tmp);
            }
            else {
                delete[] args;
                continue;
            }
        }
        else if (code[c][0] == '-') {
            if (condState == 2) {
                string tmp = code[c];
                tmp = tmp.substr(2,tmp.length()-1);
                getArgs(args,tmp);
            }
            else {
                delete[] args;
                continue;
            }
        }
        else if (code[c].find(":") != string::npos) {
            delete[] args;
            continue;
        }
        else {
            getArgs(args,code[c]);
        }
        switch (cId) {
            case 0:
                mov(args, inputs);
                delete[] args;
                break;
            case 1:
                c = lables[args[0]] - 1;
                delete[] args;
                break;
            case 2:
                add(args, inputs);
                delete[] args;
                break;
            case 3:
                sub(args, inputs);
                delete[] args;
                break;
            case 4:
                mul(args, inputs);
                delete[] args;
                break;
            case 5:
                notAcc();
                delete[] args;
                break;
            case 6:
                dgt(args, inputs);
                delete[] args;
                break;
            case 7:
                dst(args, inputs);
                delete[] args;
                break;
            case 8:
                teq(args, inputs, &condState);
                delete[] args;
                break;
            case 9:
                tgt(args, inputs, &condState);
                delete[] args;
                break;
            case 10:
                tlt(args, inputs, &condState);
                delete[] args;
                break;
            case 11:
                tcp(args, inputs, &condState);
                delete[] args;
                break;
            default:
                cerr << "no intruction found" << endl;
                delete[] args;
                return 1;
        }
    }
    delete [] inputs;
}

int findInstruction(string command) {
    for (int i = 0; i < INTR_COUNT; i++) {
        string instr = INSTRUTIONS[i];
        if (command.find(instr) != string::npos) {
            return i;
        }
    }
    return -1;
}

void getArgs(string* args,string command) {
    args[0] =  "";
    args[1] =  "";
    string tmpS = command.substr(3,command.length()-1);
    int argN = -1;
    for (int i = 0; i < tmpS.length(); i++) {
        if (argN > 1) {
            cerr << "Too many arguments!";
            break;
        }
        if (tmpS[i] == ' ') {
            argN++;
            continue;
        }
        args[argN] += tmpS[i];
    }
}

void getLables(vector<string>* code, unordered_map<string,int>* lables, int nComs) {
    for (int c = 0; c < nComs;c++) {
        if ((*code)[c].find(":") != string::npos) {
            int tmpI = (*code)[c].find(":");
            string tmpS = (*code)[c].substr(0,tmpI);
            if ((*code)[c].length() > tmpI+2) {
                (*code)[c] = (*code)[c].substr(tmpI+2,(*code)[c].length()-1);
            }
            (*lables)[tmpS] = c;
        }
    }
}

int* getArgValues(string* args,int* inputs) {
    int* argN = new int[2];
    if (args[0] == "x0") {
        argN[0] = inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "dat") {
        argN[0] = dat;
    }
    else if (args[0] == "acc") {
        argN[0] = acc;
    }
    else {
        argN[0] = stoi(args[0]);
    }
    if (args[1] == "x0") {
        argN[1] = inputs[inpInd];
        inpInd++;
    }
    else if (args[1] == "dat") {
        argN[1] = dat;
    }
    else if (args[1] == "acc") {
        argN[1] = acc;
    }
    else {
        argN[1] = stoi(args[1]);
    }
    return argN;
}

void mov(string* args, int* inputs) {
    static bool firstOut = true;
    int arg1;
    if (args[0] == "x0") {
        arg1 = inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "acc") {
        arg1 = acc;
    }
    else if (args[0] == "dat") {
        arg1 = dat;
    }
    else {
        arg1 = stoi(args[0]);
    }

    if (args[1] == "acc") {
        acc = arg1;
    }
    else if (args[1] == "dat") {
        dat = arg1;
    }
    else if (args[1] == "x1") {
        if (firstOut) {
            firstOut = false;
        }
        else {
            cout << " ";
        }
        cout << arg1;
    }
    return;
}

void add(string* args, int* inputs) {
    if (args[0] == "x0") {
        acc += inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "dat") {
        acc += dat;
    }
    else if (args[0] == "acc") {
        acc += acc;
    }
    else {
        acc += stoi(args[0]);
    }
}

void sub(string* args, int* inputs) {
    if (args[0] == "x0") {
        acc -= inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "dat") {
        acc -= dat;
    }
    else if (args[0] == "acc") {
        acc -= acc;
    }
    else {
        acc -= stoi(args[0]);
    }
}

void mul(string* args, int* inputs) {
    if (args[0] == "x0") {
        acc *= inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "dat") {
        acc *= dat;
    }
    else if (args[0] == "acc") {
        acc *= acc;
    }
    else {
        acc *= stoi(args[0]);
    }
}

void notAcc() {
    if (acc == 0) {
        acc = 100;
    }
    else {
        acc = 0;
    }
}

void dgt(string* args,int* inputs) {
    int argN;
    if (args[0] == "x0") {
        argN = inputs[inpInd];
        inpInd++;
    }
    else if (args[0] == "dat") {
        argN = dat;
    }
    else if (args[0] == "acc") {
        argN = acc;
    }
    else {
        argN = stoi(args[0]);
    }
    if (argN == 0) {
        acc %= 10;
    }
    else if (argN == 1) {
        acc /= 10;
        acc %= 10;
    }
    else if (argN == 2) {
        acc /= 100;
        acc %= 10;
    }
    else {
        cerr << "invalid argument" << endl;
    }
}

void dst (string* args, int* inputs) {
    int* argN = getArgValues(args, inputs);
    string tmpS = to_string(acc);
    while (tmpS.length() < 3) {
        tmpS = "0" + tmpS;
    }
    if (argN[0] == 0) {
        tmpS[2] = (char)argN[1] + 48;
    }
    else if (argN[0] == 1) {
        tmpS[1] = (char)argN[1] + 48;
    }
    else if (argN[0] == 2) {
        tmpS[0] = (char)argN[1] + 48;
    }
    else {
        cerr << "invalid argument" << endl;
    }
    acc = stoi(tmpS);
    delete [] argN;
    return;
}

void teq(string* args,int* inputs, int* cState) {
    int* argN = getArgValues(args, inputs);
    if (argN[0] == argN[1]) {
        *cState = 1;
    }
    else {
        *cState = 2;
    }
    delete [] argN;
    return;
}

void tgt(string* args,int* inputs, int* cState) {
    int* argN = getArgValues(args, inputs);
    if (argN[0] > argN[1]) {
        *cState = 1;
    }
    else {
        *cState = 2;
    }
    delete[] argN;
    return;
}

void tlt(string* args,int* inputs, int* cState) {
    int* argN = getArgValues(args, inputs);
    if (argN[0] < argN[1]) {
        *cState = 1;
    }
    else {
        *cState = 2;
    }
    delete[] argN;
    return;
}

void tcp(string* args,int* inputs, int* cState) {
    int* argN = getArgValues(args, inputs);
    if (argN[0] > argN[1]) {
        *cState = 1;
    }
    else if (argN[0] < argN[1]) {
        *cState = 2;
    }
    else {
        *cState = 0;
    }
    delete[] argN;
    return;
}
