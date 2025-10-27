#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<bool> active(n + 1, true);
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
            active[id] = false;
            cout << "Process " << id << " crashed.\n";
        }

        else if (ch == 2) {
            int id;
            cout << "Enter process ID to recover: ";
            cin >> id;
            active[id] = true;
            cout << "Process " << id << " recovered.\n";
        }

        else if (ch == 3) {
            int initiator;
            cout << "Enter process ID to start election: ";
            cin >> initiator;
            cout << "Election started by Process " << initiator << "\n";

            vector<int> ring;
            int current = initiator;

            do {
                if (active[current]) {
                    cout << "Process " << current << " passes the election message.\n";
                    ring.push_back(current);
                }
                current = (current % n) + 1;
            } while (current != initiator);

            int newCoordinator = *max_element(ring.begin(), ring.end());
            coordinator = newCoordinator;
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
