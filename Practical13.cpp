#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#ifdef __linux__
#define CLRSCR "clear"
#else
#define CLRSCR "cls"
#endif

using namespace std;

class memoryScheduler;

class ram
{
    int *memory;
    const int size;
    friend class memoryScheduler;

  public:
    ram(int size);
    ~ram();
};

class memoryBlock
{
    int startAddress;
    int size;
    friend class memoryScheduler;

  public:
    bool operator<(const memoryBlock &);
    bool operator>(const memoryBlock &);
    memoryBlock(int, int);
};

class process
{
    int pid;
    int ramRequirement;
    friend class memoryScheduler;
    int startAddress;
    int endAddress;

  public:
    process();
    process(int);
    bool operator==(const process &);
};

class memoryScheduler
{
    vector<process> processes;
    ram *r;
    void firstFitSchedule(process);
    void worstFitSchedule(process);
    void bestFitSchedule(process);
    void display();
    void submitProcess();
    void removeProcess();

  public:
    memoryScheduler(int);
    ~memoryScheduler();
    void simulate();
};

int main()
{
    int ramSize;
    cout << "Enter ram size: ";
    cin >> ramSize;
    memoryScheduler mschd(ramSize);
    mschd.simulate();
    return 0;
}

ram::ram(int size) : size(size)
{
    this->memory = new int[size];
    for (int i = 0; i < size; i++)
        memory[i] = -1;
}

ram::~ram()
{
    delete[] memory;
}

process::process()
{
    cout << "Enter pid of process: ";
    cin >> pid;
    cout << "Enter ram requirement: ";
    cin >> ramRequirement;
}

process::process(int pid)
{
    this->pid = pid;
    this->startAddress = 0;
    this->endAddress = 0;
}

bool process::operator==(const process &p)
{
    return this->pid == p.pid;
}

memoryBlock::memoryBlock(int startAddress, int size)
{
    this->startAddress = startAddress;
    this->size = size;
}

bool memoryBlock::operator<(const memoryBlock &m)
{
    return this->size < m.size;
}

bool memoryBlock::operator>(const memoryBlock &m)
{
    return this->size > m.size;
}

memoryScheduler::memoryScheduler(int ramSize)
{
    this->r = new ram(ramSize);
}

memoryScheduler::~memoryScheduler()
{
    delete r;
}

void memoryScheduler::firstFitSchedule(process p)
{
    int start = -1;
    vector<memoryBlock> blocks;
    for (int i = 0; i < r->size; i++)
    {
        if (start == -1 && r->memory[i] == -1)
        {
            start = i;
            continue;
        }
        if (start != -1 && (r->memory[i] != -1 || i == r->size - 1))
        {
            blocks.push_back(*(new memoryBlock(start, i - start + 1)));
            start = -1;
        }
    }
    auto iter = find_if(blocks.begin(), blocks.end(), [p](memoryBlock m) {
        return m.size >= p.ramRequirement;
    });
    auto res = find(processes.begin(), processes.end(), p);
    if (iter != blocks.end())
    {
        res->startAddress = iter->startAddress;
        res->endAddress = iter->startAddress + res->ramRequirement;
        for (int i = res->startAddress; i <= res->endAddress; i++)
            r->memory[i] = res->pid;
    }
    else
    {
        cout << "ERROR: not enough continues memory for the process" << endl;
        processes.erase(res);
    }
}

void memoryScheduler::bestFitSchedule(process p)
{
    int start = -1;
    vector<memoryBlock> blocks;
    for (int i = 0; i < r->size; i++)
    {
        if (start == -1 && r->memory[i] == -1)
        {
            start = i;
            continue;
        }
        if (start != -1 && (r->memory[i] != -1 || i == r->size - 1))
        {
            blocks.push_back(*(new memoryBlock(start, i - start)));
            start = -1;
        }
    }
    sort(blocks.begin(), blocks.end());
    auto iter = find_if(blocks.begin(), blocks.end(), [p](memoryBlock m) {
        return m.size >= p.ramRequirement;
    });
    auto res = find(processes.begin(), processes.end(), p);
    if (iter != blocks.end())
    {
        res->startAddress = iter->startAddress;
        res->endAddress = iter->startAddress + res->ramRequirement;
        for (int i = res->startAddress; i <= res->endAddress; i++)
            r->memory[i] = res->pid;
    }
    else
    {
        cout << "ERROR: not enough continues memory for the process" << endl;
        processes.erase(res);
    }
}

void memoryScheduler::worstFitSchedule(process p)
{
    int start = -1;
    vector<memoryBlock> blocks;
    for (int i = 0; i < r->size; i++)
    {
        if (start == -1 && r->memory[i] == -1)
        {
            start = i;
            continue;
        }
        if (start != -1 && (r->memory[i] != -1 || i == r->size - 1))
        {
            blocks.push_back(*(new memoryBlock(start, i - start)));
            start = -1;
        }
    }
    sort(blocks.begin(), blocks.end(), [](memoryBlock a, memoryBlock b) {
        return a.size > b.size;
    });
    auto iter = find_if(blocks.begin(), blocks.end(), [p](memoryBlock m) {
        return m.size >= p.ramRequirement;
    });
    auto res = find(processes.begin(), processes.end(), p);
    if (iter != blocks.end())
    {
        res->startAddress = iter->startAddress;
        res->endAddress = iter->startAddress + res->ramRequirement;
        for (int i = res->startAddress; i <= res->endAddress; i++)
            r->memory[i] = res->pid;
    }
    else
    {
        cout << "ERROR: not enough continues memory for the process" << endl;
        processes.erase(res);
    }
}

void memoryScheduler::simulate()
{
    int choice;
    while (1)
    {
        system(CLRSCR);
        display();
        cout << "1) Submit a process" << endl;
        cout << "2) remove a process" << endl;
        cout << "Enter 0 to exit..." << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            return;
        case 1:
            submitProcess();
            break;
        case 2:
            removeProcess();
            break;
        default:
            cout << "Error: wrong choice" << endl;
        }
        cout << "Press enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void memoryScheduler::display()
{
    cout << "MEMORY STATUS" << endl
         << endl;
    if (processes.empty())
    {
        cout << "No process in memory" << endl;
        return;
    }
    for (process p : processes)
    {
        cout << "pid: " << p.pid
             << "; start address: 0x" << hex << p.startAddress
             << "; end address: 0x" << hex << p.endAddress << endl;
    }
    cout << endl;
}

void memoryScheduler::submitProcess()
{
    process p;
    processes.push_back(p);
    int choice;
    cout << "1) use first fit algorithm" << endl;
    cout << "2) use best fit algorithm" << endl;
    cout << "3) use worst fit algorithm" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice)
    {
    default:
        cout << "INVALID CHOICE using first fit as default" << endl;
    case 1:
        firstFitSchedule(p);
        break;
    case 2:
        bestFitSchedule(p);
        break;
    case 3:
        worstFitSchedule(p);
        break;
    }
}

void memoryScheduler::removeProcess()
{
    int pid, start, end;
    cout << "Enter pid of process to remove: ";
    cin >> pid;
    auto res = find_if(processes.begin(), processes.end(), [pid](process p)
    {
        return p.pid == pid;
    });
    start = res->startAddress;
    end = res->endAddress;
    for (int i = start; i <= end; i++)
    {
        r->memory[i] = -1;
    }
    if(res!=processes.end())
        processes.erase(res);
    else
        cout<<"ERROR: process not found"<<endl;
}