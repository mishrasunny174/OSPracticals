#include <iostream>
#include <cstdlib>

#ifdef __linux__
#define CLRSCR "clear"
#else
#define CLRSCR "cls"
#endif

using namespace std;

class schedulerSimulator;

class process
{
    int arrivalTime;
    int burstTime;
    int processId;
    int waitingTime;
    int turnAroundTime;
    friend class schedulerSimulator;
    process &operator=(const process &temp);

  public:
    process();
    void run();
    void getProcessInfo();
};

class schedulerSimulator
{
    const int numberOfProcesses;
    process *queue;
    void schedule();

  public:
    schedulerSimulator(int numberOfProcesses);
    ~schedulerSimulator();
    void simulate();
};

int main()
{
    system(CLRSCR);
    int num;
    cout << "Enter number of processes to simulate: ";
    cin >> num;
    schedulerSimulator schd(num);
    schd.simulate();
    cout << "Press enter to exit..." << endl;
    cin.ignore();
    cin.get();
}

process::process()
{
    this->arrivalTime = 0;
    this->burstTime = 0;
}

void process::getProcessInfo()
{
    cout << "Enter process id: ";
    cin >> this->processId;
    cout << "Enter arrival time: ";
    cin >> this->arrivalTime;
    cout << "Enter burst time: ";
    cin >> this->burstTime;
}

void process::run()
{
    cout << "PID: " << processId
         << "\t\tburst time : " << burstTime
         << "\t\twaiting time : " << waitingTime
         << "\t\tturn around time : " << turnAroundTime
         << endl;
}

schedulerSimulator::schedulerSimulator(int numOfProcesses) : numberOfProcesses(numOfProcesses)
{
    this->queue = new process[numberOfProcesses];
    for (int i = 0; i < numberOfProcesses; i++)
    {
        cout << "Enter details of process " << i + 1 << endl;
        this->queue[i].getProcessInfo();
    }
}

schedulerSimulator::~schedulerSimulator()
{
    delete[] this->queue;
}

void schedulerSimulator::simulate()
{
    int avgWaitingTime = 0;
    int avgTurnAroundTime = 0;
    schedule();
    queue[0].waitingTime = 0;
    queue[0].turnAroundTime = queue[0].burstTime;
    for (int i = 1; i < numberOfProcesses; i++)
    {
        queue[i].waitingTime = queue[i - 1].turnAroundTime;
        queue[i].turnAroundTime = queue[i].waitingTime + queue[i].burstTime;
    }
    system(CLRSCR);
    cout<<"FCFS scheduling"<<endl<<endl;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        this->queue[i].run();
        avgWaitingTime += this->queue[i].waitingTime;
        avgTurnAroundTime += this->queue[i].turnAroundTime;
    }
    cout << endl
         << "Average turn around time: " << avgTurnAroundTime / numberOfProcesses << endl
         << "Average waiting time:  " << avgWaitingTime / numberOfProcesses << endl;
}

void schedulerSimulator::schedule()
{
    process temp;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < numberOfProcesses - 1; j++)
        {
            if (this->queue[j].arrivalTime > this->queue[j + 1].arrivalTime)
            {
                temp = this->queue[j];
                this->queue[j] = this->queue[j + 1];
                this->queue[j + 1] = temp;
            }
        }
    }
}

process &process::operator=(const process &temp)
{
    this->processId = temp.processId;
    this->burstTime = temp.burstTime;
    this->turnAroundTime = temp.turnAroundTime;
    this->waitingTime = temp.waitingTime;
    this->arrivalTime = temp.arrivalTime;
    return *this;
}