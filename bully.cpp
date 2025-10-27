#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<bool> active(n + 1, true); // process 1..n
    int coordinator = n;
    cout << "Initial Coordinator: Process " << coordinator << "\n";

    while (true) {
        cout << "\n1. Crash a process\n2. Recover a process\n3. Start election\n4. Exit\nEnter choice: ";
        int ch;
        cin >> ch;

        if (ch == 1) {
            int id;
            cout << "Enter process ID to crash: ";
            cin >> id;
            if (active[id]) 
            {
                active[id] = false;
                cout << "Process " << id << " crashed.\n";
                if (id == coordinator)
                    cout << "Coordinator crashed! Start new election.\n";
            } 
            else 
            {
                cout << "Process " << id << " already inactive.\n";
            }
        }

        else if (ch == 2) {
            int id;
            cout << "Enter process ID to recover: ";
            cin >> id;
            if (!active[id]) {
                active[id] = true;
                cout << "Process " << id << " recovered.\n";
                if (id > coordinator) {
                    coordinator = id;
                    cout << "New Coordinator after recovery: Process " << coordinator << "\n";
                }
            } else {
                cout << "Process " << id << " already active.\n";
            }
        }

        else if (ch == 3) {
            int initiator;
            cout << "Enter process ID to start election: ";
            cin >> initiator;
            cout << "Election started by Process " << initiator << "\n";

            int newC = -1;
            for (int i = initiator + 1; i <= n; i++) {
                if (active[i]) {
                    cout << "Process " << i << " responded OK.\n";
                    newC = i;
                }
            }

            if (newC == -1) {
                newC = initiator;
            }

            coordinator = newC;
            cout << "New Coordinator is Process " << coordinator << "\n";
        }

        else if (ch == 4) {
            cout << "Exiting...\n";
            break;
        }

        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
