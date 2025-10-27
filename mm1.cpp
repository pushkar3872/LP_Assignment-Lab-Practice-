#include <bits/stdc++.h>
using namespace std;

class PnTab {
public:
    unordered_map<string, int> pn;
};

class MacroPass1 {
public:
    map<string, PnTab> pntab;
    map<string, vector<vector<int>>> mnt; // macro name -> {PP, KP, MDTP, KPDTABP}
    map<int, string> mdt;
    map<string, string> kpdtab;
    vector<string> inp;

    void getwords(string pt, vector<string> &tmp) {
        stringstream ss(pt);
        string word;
        while (ss >> word) tmp.push_back(word);
    }

    void processinp() {
        int n = inp.size();
        int i = 0, mdtIndex = 1;

        while (i < n) {
            if (inp[i] == "MACRO") {
                i++;
                PnTab ppn;
                vector<string> tmp;
                getwords(inp[i], tmp);

                string macroName = tmp[0];
                int pp = 0, kp = 0;
                int kpdtp = kpdtab.size() + 1;
                int mdtp = mdtIndex;

                // Process parameters
                for (int j = 1; j < tmp.size(); j++) {
                    string ptr = tmp[j];
                    if (ptr.find('=') != string::npos) {
                        kp++;
                        string param = ptr.substr(0, ptr.find('='));
                        string def = ptr.substr(ptr.find('=') + 1);
                        kpdtab[param] = def;
                        ppn.pn[param] = ppn.pn.size() + 1;
                    } else {
                        pp++;
                        ppn.pn[ptr] = ppn.pn.size() + 1;
                    }
                }

                pntab[macroName] = ppn;
                mnt[macroName].push_back({pp, kp, mdtp, kpdtp});

                i++; // move to body

                while (i < n && inp[i] != "MEND") {
                    tmp.clear();
                    getwords(inp[i], tmp);
                    string res = tmp[0] + " ";

                    for (int k = 1; k < tmp.size(); ++k) {
                        if (tmp[k][0] == '&') {
                            string param = tmp[k];
                            int pos = ppn.pn[param];
                            res += "(p," + to_string(pos) + ")";
                        } else {
                            res += tmp[k];
                        }
                        if (k != tmp.size() - 1) res += ", ";
                    }

                    mdt[mdtIndex++] = res;
                    i++;
                }

                mdt[mdtIndex++] = "MEND";
            }
            i++;
        }
    }

    void saveTables() {
        ofstream fout;

        // Save MDT
        fout.open("MDT.txt");
        for (auto &it : mdt)
            fout << it.first << " " << it.second << "\n";
        fout.close();

        // Save MNT
        fout.open("MNT.txt");
        fout << "Name PP KP MDTP KPDTABP\n";
        for (auto &it : mnt)
            fout << it.first << " " << it.second[0][0] << " " << it.second[0][1] << " "
                 << it.second[0][2] << " " << it.second[0][3] << "\n";
        fout.close();

        // Save PNTAB
        fout.open("PNTAB.txt");
        for (auto &it : pntab) {
            fout << it.first << "\n";
            for (auto &p : it.second.pn)
                fout << p.first << " " << p.second << "\n";
            fout << "###\n";
        }
        fout.close();

        // Save KPDTAB
        fout.open("KPDTAB.txt");
        for (auto &it : kpdtab)
            fout << it.first << " " << it.second << "\n";
        fout.close();

        cout << "\n✅ Pass 1 complete — Tables saved successfully!\n";
    }
};

int main() {
    MacroPass1 mp1;
    fstream fp("myfile.txt", ios::in);

    if (!fp) {
        cout << "Error: Macro file not found!\n";
        return 1;
    }

    string st;
    while (getline(fp, st)) {
        if (!st.empty()) mp1.inp.push_back(st);
    }
    fp.close();

    mp1.processinp();
    mp1.saveTables();

    return 0;
}

/*
Input File (myfile.txt)
-----------------------
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
