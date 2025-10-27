#include <bits/stdc++.h>
using namespace std;

class Pass1 {
public:
    unordered_map<string, int> AD, CC, IS, REG;
    vector<pair<string, int>> symtab;
    vector<pair<string, int>> littab;
    vector<int> pooltab;
    vector<string> intermediate;
    int LC = 0;

    Pass1() {
        AD["START"] = 1; AD["END"] = 2; AD["ORIGIN"] = 3;
        AD["EQU"] = 4; AD["LTORG"] = 5;

        CC["LT"] = 1; CC["LE"] = 2; CC["EQ"] = 3;
        CC["GT"] = 4; CC["GE"] = 5; CC["ANY"] = 6;

        IS["STOP"] = 0; IS["ADD"] = 1; IS["SUB"] = 2;
        IS["MOVER"] = 3; IS["MOVEM"] = 4; IS["COMP"] = 5;
        IS["BC"] = 6; IS["READ"] = 7; IS["PRINT"] = 8;

        REG["AREG"] = 1; REG["BREG"] = 2;
        REG["CREG"] = 3; REG["DREG"] = 4;

        pooltab.push_back(0);
    }

    int findSymbol(string sym) {
        for (int i = 0; i < symtab.size(); ++i)
            if (symtab[i].first == sym) return i;
        return -1;
    }

    int findLiteral(string lit) {
        for (int i = 0; i < littab.size(); ++i)
            if (littab[i].first == lit) return i;
        return -1;
    }

    void assignLiteralAddresses() {
        for (int i = pooltab.back(); i < littab.size(); i++) {
            if (littab[i].second == -1) {
                littab[i].second = LC++;
            }
        }
        pooltab.push_back(littab.size());
    }

    void processLine(string line) {
        if (line.empty()) return;

        stringstream ss(line);
        vector<string> tokens;
        string temp;
        while (ss >> temp) tokens.push_back(temp);

        string label = "", opcode = "", op1 = "", op2 = "";

        // Handle label detection
        if (tokens.size() >= 2 && (IS.count(tokens[1]) || AD.count(tokens[1]) || tokens[1] == "DS" || tokens[1] == "DC")) {
            label = tokens[0];
            opcode = tokens[1];
            if (tokens.size() >= 3) op1 = tokens[2];
            if (tokens.size() >= 4) op2 = tokens[3];
        } else {
            opcode = tokens[0];
            if (tokens.size() >= 2) op1 = tokens[1];
            if (tokens.size() >= 3) op2 = tokens[2];
        }

        // START directive
        if (opcode == "START") {
            LC = stoi(op1);
            intermediate.push_back("(AD,01) (C," + to_string(LC) + ")");
            return;
        }

        // ORIGIN directive
        if (opcode == "ORIGIN") {
            LC = stoi(op1);
            intermediate.push_back("(AD,03) (C," + to_string(LC) + ")");
            return;
        }

        // EQU directive
        if (opcode == "EQU") {
            int val = findSymbol(op1) != -1 ? symtab[findSymbol(op1)].second : stoi(op1);
            int symidx = findSymbol(label);
            if (symidx == -1)
                symtab.push_back({label, val});
            else
                symtab[symidx].second = val;
            intermediate.push_back("(AD,04) (C," + to_string(val) + ")");
            return;
        }

        // LTORG or END directive
        if (opcode == "LTORG" || opcode == "END") {
            assignLiteralAddresses();
            intermediate.push_back("(AD," + string(opcode == "LTORG" ? "05" : "02") + ")");
            return;
        }

        // Declarative statements DS/DC handled separately
        if (opcode == "DS") {
            if (!label.empty()) {
                int idx = findSymbol(label);
                if (idx == -1)
                    symtab.push_back({label, LC});
                else
                    symtab[idx].second = LC;
            }
            int size = stoi(op1);
            intermediate.push_back("(DL,02) (C," + to_string(size) + ")");
            LC += size; // DS reserves memory
            return;
        }

        if (opcode == "DC") {
            if (!label.empty()) {
                int idx = findSymbol(label);
                if (idx == -1)
                    symtab.push_back({label, LC});
                else
                    symtab[idx].second = LC;
            }
            intermediate.push_back("(DL,01) (C," + op1 + ")");
            LC += 1; // DC reserves one word
            return;
        }

        // Handle label for instructions
        if (!label.empty()) {
            int symidx = findSymbol(label);
            if (symidx == -1)
                symtab.push_back({label, LC});
            else
                symtab[symidx].second = LC;
        }

        // Imperative statements
        if (IS.find(opcode) != IS.end()) {
            string ic = "(IS," + (IS[opcode] < 10 ? "0" + to_string(IS[opcode]) : to_string(IS[opcode])) + ") ";

            if (!op1.empty()) {
                if (REG.find(op1) != REG.end()) {
                    ic += "(" + to_string(REG[op1]) + ") ";
                } else if (CC.find(op1) != CC.end()) {
                    ic += "(" + to_string(CC[op1]) + ") ";
                } else {
                    int symidx = findSymbol(op1);
                    if (symidx == -1) {
                        symtab.push_back({op1, -1});
                        symidx = symtab.size() - 1;
                    }
                    ic += "(S," + to_string(symidx) + ") ";
                }
            }

            if (!op2.empty()) {
                if (op2[0] == '=') {
                    int litidx = findLiteral(op2);
                    if (litidx == -1) {
                        littab.push_back({op2, -1});
                        litidx = littab.size() - 1;
                    }
                    ic += "(L," + to_string(litidx) + ")";
                } else {
                    int symidx = findSymbol(op2);
                    if (symidx == -1) {
                        symtab.push_back({op2, -1});
                        symidx = symtab.size() - 1;
                    }
                    ic += "(S," + to_string(symidx) + ")";
                }
            }

            intermediate.push_back(ic);
            LC++;
        }
    }

    void printTables() {
        cout << "\n---------------- INTERMEDIATE CODE ----------------\n";
        for (auto& line : intermediate)
            cout << line << "\n";

        cout << "\n---------------- SYMBOL TABLE ----------------\n";
        cout << "Symbol\tAddress\n";
        for (auto& s : symtab)
            cout << s.first << "\t" << s.second << "\n";

        cout << "\n---------------- LITERAL TABLE ----------------\n";
        cout << "Literal\tAddress\n";
        for (auto& l : littab)
            cout << l.first << "\t" << l.second << "\n";

        cout << "\n---------------- POOL TABLE ----------------\n";
        cout << "Pool Index\n";
        for (auto& p : pooltab)
            cout << p << "\n";
    }
};

int main() {
    Pass1 p;
    cout << "Enter assembly code (type END to finish input):\n";

    string line;
    while (true) {
        getline(cin, line);
        p.processLine(line);
        if (line.find("END") != string::npos) break;
    }

    p.printTables();
    return 0;
}

/*
Input:
START 100
MOVER AREG =5
ADD BREG =1
MOVEM AREG ALPHA
ALPHA DS 1
END

Expected Output:

---------------- INTERMEDIATE CODE ----------------
(AD,01) (C,100)
(IS,03) (1) (L,0)
(IS,01) (2) (L,1)
(IS,04) (1) (S,0)
(DL,02) (C,1)
(AD,02)

---------------- SYMBOL TABLE ----------------
ALPHA   103

---------------- LITERAL TABLE ----------------
=5      100
=1      101

---------------- POOL TABLE ----------------
0
2
*/
