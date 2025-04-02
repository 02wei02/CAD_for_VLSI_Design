#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <set>
#include <chrono>
#include <iomanip>
#include <limits.h>
#include <cstdlib>

using namespace std;

struct ckt
{
    map<int, set<int>> nets;
    int net_count;
    string name;
    int cell_count;
};

typedef struct ckt *Ckt;

struct ab
{
    vector<set<int>> cells; // an 2D array of AB seperation blocks
    set<int> cut;           // an array of nets being cut
    int cut_size;
};

typedef struct ab *AB;

class Partitioning
{
public:
    Partitioning(string input_file, string output_file) : in(std::move(input_file)), out(std::move(output_file)) {}
    void Run()
    {
        ABnet = new ab();
        InputFile();
        SimulatedAnnealing();
        ShrinkCut(ABnet);
        OutABNet();
    }

private:
    void FirstPass(ifstream &);
    void SecondPass(ifstream &);
    void NetCount(int);
    void CellCount(int);
    void SimulatedAnnealing();
    int Cost(AB);
    void Perturb(AB, AB);
    void Initial(AB, AB);
    map<int, set<int>> DefineConnection(AB);
    void DFS(int, set<int> &, set<int> &, map<int, set<int>> &);
    void FindConnection(set<int> &, AB, map<int, set<int>> &);
    void OutABNet();
    void StartPartition(AB);
    bool IsConstraint1(AB);
    bool Metropolis(double, int);
    void ABMerge(AB);
    bool IsFindABnet(AB, int i);
    void ShrinkCut(AB);
    bool IsRedundantCut(set<int> nets, vector<set<int>> cells);

    void InputFile();
    void Output();
    void OutAB(AB);

    int CalCutSize(AB);
    bool ABsize(vector<set<int>> now_ABnet);
    Ckt circuit;

    string in;
    string out;
    int net_count = 0;
    int cell_count = 0;

    map<int, set<int>> connections;
    AB ABnet;
};

void Partitioning::InputFile()
{
    circuit = new ckt;
    ifstream inFile(in);

    circuit->name = in;

    if (!inFile)
    {
        std::cout << "Input File can't be open!" << endl;
        return;
    }

    FirstPass(inFile);
    inFile.close(); // 關閉文件
    std::cout << "Read File" << endl;
}

void Partitioning::NetCount(int net)
{
    if (net > net_count)
    {
        net_count = net;
    }
    circuit->net_count = net_count;
}

void Partitioning::CellCount(int cell)
{
    if (cell > cell_count)
    {
        cell_count = cell;
    }
    circuit->cell_count = cell_count;
}

void Partitioning::FirstPass(ifstream &inFile)
{
    string s;
    string idle;

    string net;
    while (getline(inFile, s))
    {
        istringstream iss(s);
        iss >> idle >> net;

        int netc = stoi(net.substr(1, net.size() - 1));
        NetCount(netc);

        set<int> temp;
        while (iss >> idle)
        {
            if (idle == "{")
                continue;
            if (idle == "}")
                break; // 如果是 '}' 就退出內層循環
            int idlec = stoi(idle.substr(1, idle.size() - 1));
            CellCount(idlec);

            temp.insert(idlec);
        }
        circuit->nets[netc] = temp;
    }
}

void Partitioning::Output()
{

    std::cout << right << setw(10) << "NetCount" << setw(10) << "CellCount" << endl
              << setw(10) << circuit->net_count << setw(10) << circuit->cell_count << endl;
}

int Partitioning::CalCutSize(AB now_ABnet)
{
    int sum;

    sum = now_ABnet->cut.size();

    return sum;
}

void Partitioning::OutABNet()
{
    ofstream outFile(out);

    if (!outFile)
    {
        std::cout << "Input File can't be open!" << endl;
        return;
    }
    outFile << "cut_size ";
    outFile << ABnet->cut_size << endl;
    int A = 0;
    for (const auto &net : ABnet->cells)
    {
        if (net.size() != 0)
            outFile << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            outFile << "c" << cell << endl;
        }
    }
    delete (ABnet);
}

void Partitioning::OutAB(AB now_ABnet)
{
    std::cout << "OutAB cut_size ok : ";
    std::cout << now_ABnet->cut.size() << endl;
    int A = 0;

    // 首先检查 now_ABnet->cells 是否为空
    if (!now_ABnet->cells.empty())
    {
        for (auto &net : now_ABnet->cells)
        {
            if (net.size() > 0)
                std::cout << char('A' + A) << endl;
            A++;
            for (int cell : net)
            {
                std::cout << "c" << cell << " ";
            }
            if (net.size() > 0)
                std::cout << endl;
        }
        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
        for (auto &i : now_ABnet->cut)
        {
            cout << "n" << i << " ";
        }
        cout << endl;
    }
    else
    {
        std::cout << "now_ABnet->cells is empty." << endl;
    }
}

int Partitioning::Cost(AB now_ABnet)
{
    if (now_ABnet->cells.size() >= 2)
        return now_ABnet->cells.size();
    else
        return INT_MAX;
}

// void Partitioning::Initial(AB now_ABnet, AB next_ABnet)
// {
//     *next_ABnet = *now_ABnet;
//     // Randomly choose a cell to cut
//     int chooseonecut = rand() % (circuit->net_count) + 1;

//     next_ABnet->cut.clear();
//     for (int i = 1; i <= 1; i++)
//     {
//         int r = rand() % (circuit->net_count) + 1;
//         next_ABnet->cut.insert(r);
//     }
//     next_ABnet->cut_size = CalCutSize(next_ABnet);

//     // 在新对象上执行 StartPartition
//     StartPartition(next_ABnet);
//     // std::cout << "Initial ok" << endl;
//     // OutAB(next_ABnet);
// }

void Partitioning::Initial(AB now_ABnet, AB next_ABnet)
{
    *next_ABnet = *now_ABnet;
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;

    next_ABnet->cut.clear();
    for (int i = 1; i <= chooseonecut; i++)
    {
        int r = rand() % (circuit->net_count) + 1;
        next_ABnet->cut.insert(r);
    }
    next_ABnet->cut_size = CalCutSize(next_ABnet);

    // 在新对象上执行 StartPartition
    StartPartition(next_ABnet);
    // std::cout << "Initial ok" << endl;
    // OutAB(next_ABnet);
}

void Partitioning::Perturb(AB now_ABnet, AB next_ABnet)
{
    *next_ABnet = *now_ABnet;

    int chooseonecut = rand() % (circuit->net_count) + 1;

    if (ABnet->cut_size != 0)
    {
        int i = 0;
        while (i < chooseonecut)
        {
            int r = rand() % (circuit->net_count) + 1;
            if (i <= ABnet->cut_size * 0.1)
            {
                int bef;
                bef = Cost(next_ABnet);
                if (!next_ABnet->cut.count(r))
                {
                    next_ABnet->cut.insert(r);
                }
                else
                {
                    next_ABnet->cut.erase(r);
                }
                StartPartition(next_ABnet);
                int after = Cost(next_ABnet);
                if (bef < after || !IsConstraint1(next_ABnet))
                {
                    if (next_ABnet->cut.count(r))
                        next_ABnet->cut.erase(r);
                    else
                        next_ABnet->cut.insert(r);
                }
                StartPartition(next_ABnet);
                // cout << "First ok" << endl;
                // OutAB(next_ABnet);
            }
            i++;
        }
    }
    else
    {
        int i = 1;
        while (1)
        {
            int r = rand() % (circuit->net_count) + 1;
            next_ABnet->cut.insert(r); // Mark it as cut
            StartPartition(next_ABnet);
            if (next_ABnet->cells.size() >= 2 && IsConstraint1(next_ABnet))
                break;
            i++;
            // cout << "Second ok" << endl;
            // OutAB(next_ABnet);
        }
    }
    StartPartition(next_ABnet);
    next_ABnet->cut_size = CalCutSize(next_ABnet);
    // std::cout << "Perturb ok" << endl;
    // OutAB(next_ABnet);
}

// void Partitioning::Perturb(AB now_ABnet, AB next_ABnet)
// {
//     *next_ABnet = *now_ABnet;
//     // Randomly choose a cell to cut
//     int chooseonecut = rand() % (circuit->net_count) + 1;

//     next_ABnet->cut.clear();
//     for (int i = 1; i <= chooseonecut; i++)
//     {
//         int r = rand() % (circuit->net_count) + 1;
//         next_ABnet->cut.insert(r);
//     }
//     next_ABnet->cut_size = CalCutSize(next_ABnet);

//     // 在新对象上执行 StartPartition
//     StartPartition(next_ABnet);
//     cout << "Perturb ok size" << next_ABnet->cut_size << endl;
//     for (auto &i : next_ABnet->cut)
//     {
//         cout << "n " << i << endl;
//     }
// }

void Partitioning::DFS(int current_cell, set<int> &visited, set<int> &current_net, map<int, set<int>> &next_connections)
{
    visited.insert(current_cell);
    current_net.insert(current_cell);

    for (int connected_cell : next_connections[current_cell])
    {
        if (!visited.count(connected_cell))
            DFS(connected_cell, visited, current_net, next_connections);
    }
}

void Partitioning::StartPartition(AB now_ABnet)
{
    map<int, set<int>> connection = DefineConnection(now_ABnet);
    set<int> visited;
    // std::cout << "StartPartion ok" << endl;
    FindConnection(visited, now_ABnet, connection);
    // std::cout << "ABMerge ok " << endl;
    ABMerge(now_ABnet);
    // OutAB(now_ABnet);
    visited.clear();
}

void Partitioning::FindConnection(set<int> &visited, AB next_ABnet, map<int, set<int>> &next_connections)
{
    next_ABnet->cells.clear();
    for (const auto &connection : next_connections)
    {
        int current_cell = connection.first;
        if (!visited.count(current_cell))
        {
            set<int> current_net;
            DFS(current_cell, visited, current_net, next_connections);
            next_ABnet->cells.push_back(current_net);
        }
    }
}

void Partitioning::ABMerge(AB next_ABnet)
{
    if (!next_ABnet->cells.empty())
    {
        if (next_ABnet->cells.size() > 2)
        {
            for (int i = 2; i < next_ABnet->cells.size(); i++) // 从索引 2 开始，因为索引 0 和 1 已经被初始化
            {
                if (next_ABnet->cells[0].size() < next_ABnet->cells[1].size())
                {
                    for (int cell : next_ABnet->cells[i])
                        next_ABnet->cells[0].insert(cell);
                }
                else
                {
                    for (int cell : next_ABnet->cells[i])
                        next_ABnet->cells[1].insert(cell);
                }
                next_ABnet->cells[i].clear();
            }
        }
    }

    // std::cout << "ABMerge1 ok" << endl;
    // OutAB(next_ABnet);

    // if (next_ABnet->cells.size() > 2)
    // {
    for (int i = 1; i <= circuit->cell_count; i++)
    {
        set<int> cell;
        cell.clear();
        cell.insert(i);
        if (!IsFindABnet(next_ABnet, i))
        {
            if (next_ABnet->cells.size() < 2)
                next_ABnet->cells.push_back(cell);
            else if (next_ABnet->cells[0].size() < next_ABnet->cells[1].size())
                next_ABnet->cells[0].insert(i);
            else
                next_ABnet->cells[1].insert(i);
            // std::cout << "ABLoc ok" << endl;
        }
    }
    // std::cout << "ABMerge2 ok" << endl;
    // OutAB(next_ABnet);
}

bool Partitioning::IsFindABnet(AB next_ABnet, int cell)
{
    // std::cout << "IsFindABnet ok" << endl;
    for (auto &i : next_ABnet->cells)
    {
        if (i.count(cell))
        {
            return true;
        }
    }
    // std::cout << "FindAB false ok" << endl;
    return false;
}

void Partitioning::ShrinkCut(AB now_ABnet)
{
    auto iter = now_ABnet->cut.begin();
    while (iter != now_ABnet->cut.end())
    {
        if (IsRedundantCut(circuit->nets[*iter], now_ABnet->cells))
        {
            now_ABnet->cut_size--;
            iter = now_ABnet->cut.erase(iter);
        }
        else // 不要忘记这一段
        {
            ++iter;
        }
    }
    // cout << "Final ok" << endl;
    // OutAB(now_ABnet);
}

bool Partitioning::IsRedundantCut(set<int> net, vector<set<int>> cells)
{
    int countA = 0;
    int countB = 0;
    for (auto &i : net)
    {
        if (!cells[0].count(i)) // can't find in A
            countA++;
        else
            countB++;
    }
    if (countA == 0 || countB == 0)
        return true;
    else
        return false;
}

map<int, set<int>> Partitioning::DefineConnection(AB now_ABnet)
{
    map<int, set<int>> next_connections;
    next_connections.clear();
    for (const auto &i : circuit->nets) // search all nets
    {
        int j = 0;
        if (!now_ABnet->cut.count(i.first))
        {
            for (auto &j : i.second)
            {
                set<int> temp;
                for (const auto &k : i.second)
                {
                    next_connections[j].insert(k);
                    next_connections[k].insert(j); // to construct double link
                }
            }
        }
    }
    // std::cout << "--------------------- Connection ok ---------------------------" << endl;
    // for (auto &i : next_connections)
    // {
    //     cout << i.first << "    ";
    //     for (auto &j : i.second)
    //     {
    //         cout << j << " ";
    //     }
    //     cout << endl;
    // }
    return next_connections;
}

bool Partitioning::Metropolis(double t, int cost)
{
    double r = static_cast<double>(rand()) / (RAND_MAX + 1.0);
    return (exp(-double(cost) / t) > r);
}

void Partitioning::SimulatedAnnealing()
{
    double ti = 3675, tend = 1;
    double t = ti;

    // ABnet is not empty
    if (ABnet == nullptr)
    {
        std::cout << "ABnet pointer is null." << endl;
        return;
    }
    AB now_ABnet = new ab();
    AB next_ABnet = new ab();
    ABnet->cut_size = 0;
    Initial(now_ABnet, now_ABnet); // Initialize the partition assignment

    do
    {
        // std::cout << "t ok" << endl;
        do
        {
            Perturb(now_ABnet, next_ABnet);
            // std::cout << "Perturb cut ok" << endl;
            // std::cout << "nowAB_cutsize ok" << now_ABnet->cut_size << endl
            //           << endl
            //           << endl;

            // std::cout << "Perturb ok now" << endl;
            // OutAB(now_ABnet);
            // std::cout << "Perturb ok next" << endl;
            // OutAB(next_ABnet);

            if (Cost(next_ABnet) <= Cost(now_ABnet) && IsConstraint1(now_ABnet))
            {
                *now_ABnet = *next_ABnet;
                if (Cost(now_ABnet) <= Cost(ABnet) && IsConstraint1(now_ABnet))
                {
                    *ABnet = *now_ABnet;
                    std::cout << "ok best" << endl;
                    // OutAB(ABnet);
                }
            }
            else if (Metropolis(t, Cost(next_ABnet) - Cost(now_ABnet)))
            {
                *now_ABnet = *next_ABnet;
            }
        } while (!IsConstraint1(ABnet));
        // std::cout << "temperature ok " << t << endl;
        t = 0.9 * t;
    } while ((t > tend));

    // OutAB(ABnet);
    // std::cout
    //     << "Simulated end" << endl;

    delete now_ABnet;
    delete next_ABnet;
}

// bool Partitioning::ABsize(vector<set<int>> now_ABnet)
// {
//     int total_size = 0;
//     if (!now_ABnet.empty()) // 检查ABnet不为空且大小至少为2
//     {
//         if (now_ABnet.size() == 2)
//         {
//             for (auto &i : now_ABnet)
//             {
//                 for (auto &j : i)
//                     total_size++;
//             }
//             if (total_size == circuit->cell_count)
//             {
//                 return true;
//             }
//         }
//     }
//     return false;
// }

bool Partitioning::IsConstraint1(AB now_ABnet)
{
    if (now_ABnet->cells.size() < 2)
    {
        // std::cout << "Not enough sets in now_ABnet" << endl;
        return false;
    }
    // std::cout << "IsConstraint1 ok" << endl;

    if (now_ABnet->cells[0].empty() || now_ABnet->cells[1].empty())
    {
        // std::cout << "One of the sets is empty" << endl;
        return false;
    }
    else
    {

        int countA = now_ABnet->cells[0].size();
        int countB = now_ABnet->cells[1].size();

        // std::cout << "Both sets are non-empty" << endl;
        // Use integer comparison to avoid precision issues
        if (double(abs(countA - countB)) <= double(circuit->cell_count) / 5)
        {
            // std::cout << "Difference in cell counts is within the threshold ok " << endl
            //           << endl;
            return true;
        }
        else
        {
            // std::cout << "Difference in cell counts exceeds the threshold" << endl;
            return false;
        }
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc != 3)
    {
        std::cout << "Can't open!" << endl;
        return 1;
    }

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    Partitioning partition(argv[1], argv[2]);
    partition.Run();

    return 0;
}
