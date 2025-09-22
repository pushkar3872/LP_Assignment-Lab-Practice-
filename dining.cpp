#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;

const int NUM_PHILOSOPHERS = 5;
mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int i = 0; i < 3; i++) { // each philosopher eats 3 times
        // Think
        cout << "Philosopher " << id << " is thinking.\n";
        this_thread::sleep_for(chrono::milliseconds(500));

        // Pick up forks (avoid deadlock: pick lower-numbered fork first)
        if (id % 2 == 0) {
            forks[left].lock();
            forks[right].lock();
        } else {
            forks[right].lock();
            forks[left].lock();
        }

        // Eat
        cout << "Philosopher " << id << " is eating.\n";
        this_thread::sleep_for(chrono::milliseconds(500));

        // Put down forks
        forks[left].unlock();
        forks[right].unlock();

        cout << "Philosopher " << id << " finished eating.\n";
    }
}

int main() {
    vector<thread> philosophers;

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.push_back(thread(philosopher, i));
    }

    // Wait for all philosophers to finish
    for (auto& t : philosophers) {
        t.join();
    }

    cout << "All philosophers have finished eating.\n";
    return 0;
}
