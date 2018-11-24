#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#ifdef __linux__
#define CLRSCR "clear"
#else
#define CLRSCR "cls"
#endif

using namespace std;

class RoundRobinScheduler;

class process
{
    int processId;
    int arrivalTime;
    int burstTime;
    int turnAroundTime;
    int waitingTime;
    int runTime;
    bool isPresentInReadyQueue;
    friend class RoundRobinScheduler;

  public:
    process();
    bool operator<(const process &);
    void show();
};

class RoundRobinScheduler
{
    queue<process> jobQueue;
    queue<process> readyQueue;
    vector<process> tasks;
    const int timeSlice;
    const int totalProcesses;
    int totalBurstTime;
    void schedule();
    void contextSwitch();
    int calculateAverageWaitingTime();
    int calculateAverageTurnAroundTime();

  public:
    RoundRobinScheduler(int timeSlice, int totalProcesses);
    void simulate();
};

int main()
{
    system(CLRSCR);
    int numOfProcesses, timeSlice;
    cout << "Enter number of processes to simulate: ";
    cin >> numOfProcesses;
    cout << "Enter time slice: ";
    cin >> timeSlice;
    RoundRobinScheduler sched(timeSlice, numOfProcesses);
    sched.simulate();
    cout << "Press enter to continue...";
    cin.ignore();
    cin.get();
    return 0;
}

process::process()
{
    cout << "Enter process id: ";
    cin >> processId;
    cout << "Enter arrival time: ";
    cin >> arrivalTime;
    cout << "Enter burst time: ";
    cin >> burstTime;
    waitingTime = 0;
    runTime = 0;
    turnAroundTime = 0;
    isPresentInReadyQueue = false;
}

bool process::operator<(const process &p)
{
    return this->arrivalTime < p.arrivalTime;
}

void process::show()
{
    cout << "pid: " << processId
         << "\t burst time: " << burstTime
         << "\t waitingTime: " << waitingTime
         << "\t turn around time: " << turnAroundTime
         << endl;
}

RoundRobinScheduler::RoundRobinScheduler(int timeSlice, int totalProcesses) : timeSlice(timeSlice), totalProcesses(totalProcesses)
{
    for (int i = 0; i < totalProcesses; i++)
    {
        cout << "Enter details of process " << i + 1 << endl;
        process p;
        tasks.push_back(p);
    }
    totalBurstTime = 0;
}

void RoundRobinScheduler::schedule()
{
    sort(tasks.begin(), tasks.end());
}

void RoundRobinScheduler::simulate()
{
    system(CLRSCR);
    schedule();
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
    {
        totalBurstTime += iter->burstTime;
        jobQueue.push(*iter);
    }
    if (jobQueue.front().arrivalTime != 0)
        totalBurstTime += jobQueue.front().arrivalTime;
    for (int i = 0; i < totalBurstTime; i++)
    {
        // loading processes in ready queue
        while (!jobQueue.empty() && jobQueue.front().arrivalTime == i)
        {
            readyQueue.push(jobQueue.front());
            for(auto iter = tasks.begin();iter<=tasks.end();iter++)
            {
                if (iter->processId == jobQueue.front().processId)
                    iter->isPresentInReadyQueue=true;
            }
            jobQueue.pop();
        }
        if (i % timeSlice == 0 || readyQueue.front().runTime == readyQueue.front().burstTime)
        {
            if (readyQueue.front().runTime == readyQueue.front().burstTime)
            {
                readyQueue.pop();
            }
            else
                contextSwitch();
        }
        readyQueue.front().runTime++;
        // cout<<"running "<<readyQueue.front().processId<<endl;
        for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
        {
            if (iter->runTime == iter->burstTime)
                continue;
            if (iter->processId != readyQueue.front().processId)
            {
                if(iter->isPresentInReadyQueue)
                    iter->waitingTime++;
            }
            else
                iter->runTime++;
        }
    }
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
    {
        iter->turnAroundTime = iter->waitingTime + iter->burstTime;
        iter->show();
    }
    cout << endl
         << "average waiting time: " << calculateAverageWaitingTime() << endl
         << "average turn around time: " << calculateAverageTurnAroundTime() << endl;
}

void RoundRobinScheduler::contextSwitch()
{
    readyQueue.push(readyQueue.front());
    readyQueue.pop();
}

int RoundRobinScheduler::calculateAverageWaitingTime()
{
    int sum=0;
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
        sum+=iter->waitingTime;
    return sum/totalProcesses;
}

int RoundRobinScheduler::calculateAverageTurnAroundTime()
{
    int sum=0;
    for (auto iter = tasks.begin(); iter != tasks.end(); iter++)
        sum+=iter->turnAroundTime;
    return sum/totalProcesses;
}