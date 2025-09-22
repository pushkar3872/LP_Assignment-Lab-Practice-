#include <bits/stdc++.h>
using namespace std;

class PnTab {
public:
    unordered_map<string, int> pn;
};

class MacroPass1 {
public:
    map<string, PnTab> pntab;
    map<string, vector<vector<int>>> mnt; // macro name -> PP, KP, MDT_P, KPDTAB_P
    map<int, string> mdt;
    map<string, string> kpdtab;
    vector<string> inp;
    map<string, int> isres;

    vector<string> macrocall;
    vector<string> macroexpand;

    MacroPass1() {
        isres["MOVER"]++;
        isres["ADD"]++;
        isres["MEND"]++;

        // Macro call examples
        string tp = "M1 10 20 &B=CREG";
        string pt = "M2 100 200 &V=AREG &U=BREG";

        macrocall.push_back(tp);
        macrocall.push_back(pt);
    }

    void printresult() {
        // Print MDT
        cout << "\n--- MDT (Macro Definition Table) ---\n";
        for (auto it : mdt) {
            cout << it.first << " " << it.second << "\n";
        }

        // Print MNT
        cout << "\n--- MNT (Macro Name Table) ---\n";
        cout << "Name\t#PP\t#KP\tMDTP\tKPDTABP\n";
        for (auto &it : mnt) {
            cout << it.first << "\t";
            for (int val : it.second[0]) {
                cout << val << "\t";
            }
            cout << "\n";
        }

        // Print PNTAB
        cout << "\n--- PNTAB (Parameter Name Table) ---\n";
        for (auto &macro : pntab) {
            cout << macro.first << ":\n";
            for (auto &param : macro.second.pn) {
                cout << param.first << " -> " << param.second << "\n";
            }
        }

        // Print KPDTAB
        cout << "\n--- KPDTAB (Keyword Parameter Default Table) ---\n";
        int i = 1;
        for (auto &it : kpdtab) {
            cout << i++ << ") " << it.first << " = " << it.second << "\n";
        }
    }

    void getwords(string pt, vector<string> &tmp) {
        stringstream ss(pt);
        string word;
        while (ss >> word) {
            tmp.push_back(word);
        }
    }

    void processinp() {
        int n = (int)inp.size();
        int i = 0;
        int mdtIndex = 1;

        while (i < n) {
            if (inp[i] == "MACRO") {
                i++;
                PnTab ppn;
                vector<string> tmp;
                getwords(inp[i], tmp);

                string macroName = tmp[0];
                int pp = 0, kp = 0;
                int kpdtp = (int)kpdtab.size() + 1;
                int mdtp = mdtIndex;

                // Process parameters
                for (int j = 1; j < (int)tmp.size(); j++) {
                    string ptr = tmp[j];
                    if (ptr.find('=') != string::npos) {
                        kp++;
                        string param = ptr.substr(0, ptr.find('='));
                        string def = ptr.substr(ptr.find('=') + 1);
                        kpdtab[param] = def;
                        ppn.pn[param] = (int)ppn.pn.size() + 1;
                    } else {
                        pp++;
                        ppn.pn[ptr] = (int)ppn.pn.size() + 1;
                    }
                }

                pntab[macroName] = ppn;
                mnt[macroName].push_back({pp, kp, mdtp, kpdtp});

                i++; // move to macro body

                while (i < n && inp[i] != "MEND") {
                    tmp.clear();
                    getwords(inp[i], tmp);

                    if ((int)tmp.size() < 1) {
                        cout << "Invalid macro line: " << inp[i] << "\n";
                        i++;
                        continue;
                    }

                    string res = tmp[0] + " ";

                    for (int k = 1; k < (int)tmp.size(); ++k) {
                        if (tmp[k][0] == '&') {
                            string param = tmp[k];
                            int pos = ppn.pn[param];
                            res += "(p," + to_string(pos) + ")";
                        } else {
                            res += tmp[k];
                        }

                        if (k != (int)tmp.size() - 1)
                            res += ", ";
                    }

                    mdt[mdtIndex++] = res;
                    i++;
                }

                mdt[mdtIndex++] = "MEND";
                i++;
            } else {
                i++;
            }
        }
    }

    vector<string> getsep(string tp) {
        vector<string> tmp;
        string st = "";
        int i = 0;
        while (i < (int)tp.size()) {
            if (tp[i] == ' ') {
                if (st.length() > 0) {
                    tmp.push_back(st);
                }
                st = "";
            } else {
                st += tp[i];
            }
            i++;
        }
        if (st.length() > 0) {
            tmp.push_back(st);
        }
        return tmp;
    }

    // --- Updated handlemacrocall ---
    string handlemacrocall(string mcall) {
        map<string, string> aptab; // argument parameter table: param -> value
        vector<string> tmp = getsep(mcall);

        // Find macro info from MNT
        vector<int> mn; // PP, KP, MDT_P, KPDTAB_P
        for (auto &it : mnt) {
            if (it.first == tmp[0]) {
                mn = it.second[0];
                break;
            }
        }

        int pp = mn[0];    // positional params count
        int kp = mn[1];    // keyword params count
        int mdtp = mn[2];  // MDT start index
        int kpdtp = mn[3]; // KPDTAB start index (not used here)

        // Initialize aptab with default keyword parameter values (from KPDTAB order)
        // Extract KP params from pntab for macro (to get keys and order)
        vector<pair<string, int>> params; // paramName, pos
        for (auto &p : pntab[tmp[0]].pn)
            params.push_back({p.first, p.second});
        sort(params.begin(), params.end(), [](auto &a, auto &b) { return a.second < b.second; });

        // Initialize all params to empty first
        for (auto &p : params) {
            aptab[p.first] = "";
        }

        // Set default values for KP params from kpdtab
        int kpCount = 0;
        for (auto &p : params) {
            // Check if param exists in kpdtab (i.e. is a keyword param)
            if (kpdtab.find(p.first) != kpdtab.end()) {
                aptab[p.first] = kpdtab[p.first];
                kpCount++;
                if (kpCount == kp) break;
            }
        }

        // Fill positional parameters from macro call arguments
        for (int i = 1; i <= pp && i < (int)tmp.size(); i++) {
            aptab[params[i - 1].first] = tmp[i];
        }

        // Fill keyword parameters from macro call arguments (param=value pairs)
        for (int i = pp + 1; i < (int)tmp.size(); i++) {
            string arg = tmp[i];
            size_t eq_pos = arg.find('=');
            if (eq_pos != string::npos) {
                string param_name = arg.substr(0, eq_pos);
                string val = arg.substr(eq_pos + 1);
                if (aptab.find(param_name) != aptab.end()) {
                    aptab[param_name] = val;
                }
            }
        }

        // Now expand MDT lines starting at mdtp until MEND
        stringstream expanded;
        int cur = mdtp;
        while (mdt[cur] != "MEND") {
            string line = mdt[cur];
            string new_line;
            for (size_t i = 0; i < line.size(); i++) {
                if (line[i] == '(' && i + 3 < line.size() && line[i + 1] == 'p' && line[i + 2] == ',') {
                    i += 3;
                    string num_str;
                    while (i < line.size() && isdigit(line[i]))
                        num_str += line[i++];
                    if (i < line.size() && line[i] == ')') {
                        int pos = stoi(num_str);
                        // Find param name by position
                        string param_name = "";
                        for (auto &pr : params) {
                            if (pr.second == pos) {
                                param_name = pr.first;
                                break;
                            }
                        }
                        // Replace with actual value from aptab
                        if (aptab.find(param_name) != aptab.end()) {
                            new_line += aptab[param_name];
                        } else {
                            new_line += "(p," + num_str + ")"; // fallback if param not found
                        }
                    }
                } else {
                    new_line += line[i];
                }
            }
            expanded << new_line << "\n";
            cur++;
        }

        return expanded.str();
    }

    // --- Updated macropass2 ---
    void macropass2() {
        for (int i = 0; i < (int)macrocall.size(); i++) {
            string res = handlemacrocall(macrocall[i]);
            macroexpand.push_back(res);
        }
    }
};

int main() {

    MacroPass1 mp1;
    fstream fp;
    fp.open("myfile.txt", ios::in | ios::out); // Correct file open

    if (!fp) {
        cout << "Error: File not found!\n";
        return 1;
    }

    string st;
    while (getline(fp, st)) {
        if (!st.empty()) {
            mp1.inp.push_back(st);
        }
    }

    fp.close(); // good practice to close the file

    mp1.processinp();

    // Print Pass 1 output tables
    mp1.printresult();

    // Now perform pass 2 - macro expansions for calls
    mp1.macropass2();

    // Print expanded macros after pass 2
    cout << "\n--- Macro Expansions (Pass 2 output) ---\n";
    int callNum = 1;
    for (auto &exp : mp1.macroexpand) {
        cout << "Macro Call " << callNum++ << " expansion:\n";
        cout << exp << "\n";
    }

    return 0;
}


/*
MACRO
M1 &X &Y &A=AREG &B=
MOVER &A &X
ADD &A 1
MOVER &A &Y
ADD &A 5
MEND
MACRO
M2 &P &Q &U=CREG &V=DREG
MOVER &U &P
MOVER &V &Q
ADD &U 15
ADD &V 10
MEND
*/
/*
--- MDT (Macro Definition Table) ---
1 MOVER (p,3), (p,1)
2 ADD (p,3), 1
3 MOVER (p,3), (p,2)
4 ADD (p,3), 5
5 MEND
6 MOVER (p,3), (p,1)
7 MOVER (p,4), (p,2)
8 ADD (p,3), 15
9 ADD (p,4), 10
10 MEND
*/