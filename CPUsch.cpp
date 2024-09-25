#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

struct Process {
    int pid;  // Process ID
    int arrivalTime;  // Arrival time
    int burstTime;  // Burst time
    int priority;  // Priority for Priority Scheduling
    int completionTime, waitingTime, turnaroundTime;  // Computed times
};

// Comparator for FCFS (First Come First Serve) based on Arrival Time
bool compareArrivalTime(Process a, Process b) {
    return a.arrivalTime < b.arrivalTime;
}

// Comparator for SJF (Shortest Job First) based on Burst Time
bool compareBurstTime(Process a, Process b) {
    if (a.burstTime == b.burstTime)
        return a.arrivalTime < b.arrivalTime;
    return a.burstTime < b.burstTime;
}

// Comparator for Priority Scheduling
bool comparePriority(Process a, Process b) {
    if (a.priority == b.priority)
        return a.arrivalTime < b.arrivalTime;
    return a.priority < b.priority;
}

// FCFS Scheduling
void fcfsScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrivalTime);
    int currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].completionTime = currentTime + processes[i].burstTime;
        currentTime = processes[i].completionTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }
}

// SJF Scheduling
void sjfScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareBurstTime);
    int currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].completionTime = currentTime + processes[i].burstTime;
        currentTime = processes[i].completionTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }
}

// Priority Scheduling
void priorityScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), comparePriority);
    int currentTime = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].completionTime = currentTime + processes[i].burstTime;
        currentTime = processes[i].completionTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }
}

// Round Robin Scheduling
void roundRobinScheduling(vector<Process>& processes, int quantum) {
    queue<int> q;
    vector<int> remainingBurstTime(processes.size());
    vector<bool> inQueue(processes.size(), false);

    for (int i = 0; i < processes.size(); i++) {
        remainingBurstTime[i] = processes[i].burstTime;
    }

    int currentTime = 0;
    q.push(0);
    inQueue[0] = true;

    while (!q.empty()) {
        int idx = q.front();
        q.pop();

        if (remainingBurstTime[idx] <= quantum) {
            currentTime += remainingBurstTime[idx];
            remainingBurstTime[idx] = 0;
            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        } else {
            currentTime += quantum;
            remainingBurstTime[idx] -= quantum;
        }

        for (int i = 0; i < processes.size(); i++) {
            if (i != idx && remainingBurstTime[i] > 0 && processes[i].arrivalTime <= currentTime && !inQueue[i]) {
                q.push(i);
                inQueue[i] = true;
            }
        }

        if (remainingBurstTime[idx] > 0) {
            q.push(idx);
        }
    }
}

// Print Process Info
void printProcesses(vector<Process> processes) {
    cout << "Process no.\tArrival Time\tBurst Time\tCompletion Time\t\tTurnaround Time\t\tWaiting Time\n";
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].pid << "\t\t"
             << processes[i].arrivalTime << "\t\t"
             << processes[i].burstTime << "\t\t"
             << processes[i].completionTime << "\t\t\t"
             << processes[i].turnaroundTime << "\t\t\t"
             << processes[i].waitingTime << endl;
    }
}

int main() {
    int n, quantum, choice;
    vector<Process> processes;

        cout << "\nWelcome Dear Programmer!\n";

    while (true) {
        cout << "\nEnter number of processes (Enter 0 to quit): ";
        cin >> n;

        if (n == 0) {
            cout << "Exiting the program.\n";
            break;
        }

        processes.resize(n);
        for (int i = 0; i < n; i++) {
            processes[i].pid = i+1;
            cout << "Enter arrival time, burst time, and priority for process " << i + 1 << ": ";
            cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        }

        cout << "Choose scheduling algorithm:\n1. FCFS\n2. SJF\n3. Priority\n4. Round-Robin\n";
        cin >> choice;

        switch(choice) {
            case 1:
                fcfsScheduling(processes);
                break;
            case 2:
                sjfScheduling(processes);
                break;
            case 3:
                priorityScheduling(processes);
                break;
            case 4:
                cout << "Enter time quantum for Round-Robin: ";
                cin >> quantum;
                roundRobinScheduling(processes, quantum);
                break;
            default:
                cout << "Invalid choice!";
                continue;
        }

        printProcesses(processes);
    }

    return 0;
}
