#include <bits/stdc++.h>
using namespace std;

struct Symbol {
    string name;
    int addr;
};

struct Literal {
    string value;
    int addr;
};

class Pass1 {
public:
    map<string, int> MOT; // Machine Op Table
    map<string, int> IS;  // Imperative Statements
    map<string, int> DL;  // Declarative Statements
    map<string, int> AD;  // Assembler Directives

    vector<Symbol> symtab;
    vector<Literal> littab;
    vector<int> pooltab;

    int LC; // Location Counter

    Pass1() {
        initTables();
        LC = 0;
        pooltab.push_back(0);
    }

    void initTables() {
        IS["STOP"] = 0; IS["ADD"] = 1; IS["SUB"] = 2;
        IS["MULT"] = 3; IS["MOVER"] = 4; IS["MOVEM"] = 5;
        IS["COMP"] = 6; IS["BC"] = 7; IS["DIV"] = 8;
        IS["READ"] = 9; IS["PRINT"] = 10;

        DL["DC"] = 1; DL["DS"] = 2;

        AD["START"] = 1; AD["END"] = 2;
        AD["ORIGIN"] = 3; AD["EQU"] = 4; AD["LTORG"] = 5;
    }

    int searchSym(string s) {
        for (int i = 0; i < (int)symtab.size(); i++) {
            if (symtab[i].name == s) return i;
        }
        return -1;
    }

    int searchLit(string s) {
        for (int i = 0; i < (int)littab.size(); i++) {
            if (littab[i].value == s && littab[i].addr == -1) return i;
        }
        return -1;
    }

    void processLine(string line) {
        vector<string> tokens;
        stringstream ss(line);
        string word;
        while (ss >> word) tokens.push_back(word);

        if (tokens.empty()) return;

        int i = 0;
        string label = "", opcode = "", operand1 = "", operand2 = "";

        if (IS.count(tokens[i]) || DL.count(tokens[i]) || AD.count(tokens[i])) {
            opcode = tokens[i++];
        } else {
            label = tokens[i++];
            if (i < (int)tokens.size()) opcode = tokens[i++];
        }
        if (i < (int)tokens.size()) operand1 = tokens[i++];
        if (i < (int)tokens.size()) operand2 = tokens[i++];

        if (!label.empty()) {
            int idx = searchSym(label);
            if (idx == -1) symtab.push_back({label, LC});
            else symtab[idx].addr = LC;
        }

        if (opcode.empty()) return;

        if (IS.count(opcode)) {
            string ic = string("(IS,") + (IS[opcode] < 10 ? "0" : "") + to_string(IS[opcode]) + ") ";

            if (!operand1.empty()) {
                if (operand1 == "AREG") ic += "(RG,01) ";
                else if (operand1 == "BREG") ic += "(RG,02) ";
                else if (operand1 == "CREG") ic += "(RG,03) ";
                else if (operand1 == "DREG") ic += "(RG,04) ";
                else {
                    int idx = searchSym(operand1);
                    if (idx == -1) {
                        symtab.push_back({operand1, -1});
                        idx = symtab.size() - 1;
                    }
                    ic += "(S," + to_string(idx + 1) + ") ";
                }
            }

            if (!operand2.empty()) {
                if (operand2[0] == '=') {
                    int idx = searchLit(operand2);
                    if (idx == -1) {
                        littab.push_back({operand2, -1});
                        idx = littab.size() - 1;
                    }
                    ic += "(L," + to_string(idx + 1) + ")";
                } else {
                    int idx = searchSym(operand2);
                    if (idx == -1) {
                        symtab.push_back({operand2, -1});
                        idx = symtab.size() - 1;
                    }
                    ic += "(S," + to_string(idx + 1) + ")";
                }
            }

            cout << LC << " " << ic << endl;
            LC++;
        }
        else if (DL.count(opcode)) {
            if (opcode == "DS") {
                int size = stoi(operand1);
                cout << LC << " (DL,02) (C," << size << ")" << endl;
                LC += size;
            } else if (opcode == "DC") {
                cout << LC << " (DL,01) (C," << operand1 << ")" << endl;
                LC++;
            }
        }
        else if (AD.count(opcode)) {
            if (opcode == "START") {
                LC = stoi(operand1);
                cout << "    (AD,01) (C," << operand1 << ")" << endl;
            } else if (opcode == "END") {
                for (int i = pooltab.back(); i < (int)littab.size(); i++) {
                    littab[i].addr = LC++;
                }
                pooltab.push_back(littab.size());
                cout << "    (AD,02)" << endl;
            }
        }
    }

    void pass1(vector<string>& program) {
        for (auto line : program) processLine(line);
    }

    void printTables() {
        cout << "\n--- SYMBOL TABLE ---\n";
        for (int i = 0; i < (int)symtab.size(); i++)
            cout << i + 1 << " " << symtab[i].name << " " << symtab[i].addr << endl;

        cout << "\n--- LITERAL TABLE ---\n";
        for (int i = 0; i < (int)littab.size(); i++)
            cout << i + 1 << " " << littab[i].value << " " << littab[i].addr << endl;

        cout << "\n--- POOL TABLE ---\n";
        for (int i = 0; i < (int)pooltab.size(); i++)
            cout << "#" << i + 1 << " -> " << pooltab[i] + 1 << endl;
    }
};

int main() {
    vector<string> program = {
        "START 100",
        "MOVER AREG, =5",
        "ADD BREG, =1",
        "MOVEM AREG, ALPHA",
        "ALPHA DS 1",
        "END"
    };

    Pass1 p;
    p.pass1(program);
    p.printTables();

    return 0;
}


/*

input pass1- assembler:

START 100
MOVER AREG, =5
ADD BREG, =1
MOVEM AREG, ALPHA
ALPHA DS 1
END


intermediate code generation:

    (AD,01) (C,100)
100 (IS,04) (RG,01) (L,1)
101 (IS,01) (RG,02) (L,2)
102 (IS,05) (RG,01) (S,1)
103 (DL,02) (C,1)
    (AD,02)




*/