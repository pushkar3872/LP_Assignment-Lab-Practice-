#include <bits/stdc++.h>
using namespace std;

class MacroPass2 {
public:
    map<string, vector<int>> mnt; // PP, KP, MDTP, KPDTABP
    map<int, string> mdt;
    map<string, string> kpdtab;
    map<string, unordered_map<string, int>> pntab;
    vector<string> macrocalls;

    void loadTables() {
        ifstream fin;
        string name;

        // Load MDT
        fin.open("MDT.txt");
        int idx; string line;
        while (fin >> idx) {
            getline(fin, line);
            mdt[idx] = line.substr(1);
        }
        fin.close();

        // Load MNT
        fin.open("MNT.txt");
        string header;
        getline(fin, header);
        string macro; int pp, kp, mdtp, kpdtp;
        while (fin >> macro >> pp >> kp >> mdtp >> kpdtp)
            mnt[macro] = {pp, kp, mdtp, kpdtp};
        fin.close();

        // Load KPDTAB
        fin.open("KPDTAB.txt");
        string key, val;
        while (fin >> key >> val)
            kpdtab[key] = val;
        fin.close();

        // Load PNTAB
        fin.open("PNTAB.txt");
        string line2, pname;
        while (getline(fin, line2)) {
            if (line2.empty()) continue;
            if (line2 == "###") continue;
            stringstream ss(line2);
            ss >> name;
            while (getline(fin, line2)) {
                if (line2 == "###") break;
                string param; int pos;
                stringstream ps(line2);
                ps >> param >> pos;
                pntab[name][param] = pos;
            }
        }
        fin.close();

        cout << "✅ Pass 2 loaded tables successfully!\n";
    }

    vector<string> split(string s) {
        vector<string> v;
        string tmp;
        stringstream ss(s);
        while (ss >> tmp) v.push_back(tmp);
        return v;
    }

    string expandMacro(string call) {
        vector<string> parts = split(call);
        string mname = parts[0];
        int pp = mnt[mname][0];
        int kp = mnt[mname][1];
        int mdtp = mnt[mname][2];

        map<string, string> aptab;
        vector<pair<string,int>> params;

        for (auto &p : pntab[mname]) params.push_back({p.first, p.second});
        sort(params.begin(), params.end(), [](auto &a, auto &b){ return a.second < b.second; });

        // Set defaults for keyword params
        for (auto &p : params)
            if (kpdtab.find(p.first) != kpdtab.end())
                aptab[p.first] = kpdtab[p.first];

        // Assign positional params
        for (int i = 1; i <= pp && i < parts.size(); i++)
            aptab[params[i-1].first] = parts[i];

        // Keyword params in call
        for (int i = pp + 1; i < parts.size(); i++) {
            if (parts[i].find('=') != string::npos) {
                string k = parts[i].substr(0, parts[i].find('='));
                string v = parts[i].substr(parts[i].find('=') + 1);
                aptab[k] = v;
            }
        }

        // Expand macro
        stringstream out;
        int cur = mdtp;
        while (mdt[cur] != "MEND") {
            string line = mdt[cur];
            for (auto &p : params) {
                string findStr = "(p," + to_string(p.second) + ")";
                size_t pos = line.find(findStr);
                if (pos != string::npos)
                    line.replace(pos, findStr.size(), aptab[p.first]);
            }
            out << line << "\n";
            cur++;
        }
        return out.str();
    }
};

int main() {
    MacroPass2 mp2;
    mp2.loadTables();

    // Example macro calls
    mp2.macrocalls = {
        "M1 10 20 &B=CREG",
        "M2 100 200 &V=AREG &U=BREG"
    };

    cout << "\n--- Expanded Macros (Pass 2 Output) ---\n";
    for (auto &m : mp2.macrocalls) {
        cout << "\nCall: " << m << "\n";
        cout << mp2.expandMacro(m);
    }

    return 0;
}
