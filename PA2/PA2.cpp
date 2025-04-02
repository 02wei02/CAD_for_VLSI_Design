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

using namespace std;

struct ckt
{
    map<int, vector<int>> nets;
    int net_count;
    string name;
    int cell_count;
};

typedef struct ckt *Ckt;

struct ab
{
    vector<set<int>> cells;
    set<int> cut;
    int cut_size;
};

class Partitioning
{
public:
    Partitioning(string input_file, string output_file) : in(std::move(input_file)), out(std::move(output_file)) {}
    void Run()
    {
        InputFile();
        SimulatedAnnealing();
        OutABNet();
    }

private:
    void FirstPass(ifstream &);
    void SecondPass(ifstream &);
    void NetCount(int);
    void CellCount(int);
    void SimulatedAnnealing();
    int Cost(ab);
    ab Perturb(ab &);
    ab Initial(ab &);
    map<int, vector<int>> DefineConnection(ab);
    void DFS(int, set<int> &, set<int> &, map<int, vector<int>>);
    void FindConnection(set<int> &, ab &, map<int, vector<int>>);
    void OutABNet();
    void StartPartition(ab &);
    bool Constraint1(ab);
    bool Metropolis(double, int);
    void ABMerge(ab &);
    bool FindInABnet(ab, int i);

    void InputFile();
    void Output();
    void OutAB(ab);

    int CalCutSize(ab);
    bool ABsize(vector<set<int>> now_ABnet);
    Ckt circuit;

    string in;
    string out;
    int net_count = 0;
    int cell_count = 0;

    map<int, vector<int>> connections;
    ab ABnet;
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
    cout << "Read File" << endl;
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

        vector<int> temp;
        while (iss >> idle)
        {
            if (idle == "{")
                continue;
            if (idle == "}")
                break; // 如果是 '}' 就退出內層循環
            int idlec = stoi(idle.substr(1, idle.size() - 1));
            CellCount(idlec);

            temp.push_back(idlec);
        }
        circuit->nets[netc] = temp;
    }
}

void Partitioning::Output()
{

    std::cout << right << setw(10) << "NetCount" << setw(10) << "CellCount" << endl
              << setw(10) << circuit->net_count << setw(10) << circuit->cell_count << endl;
}

int Partitioning::CalCutSize(ab now_ABnet)
{
    int sum = 0;
    sum = now_ABnet.cut.size();

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
    outFile << ABnet.cut_size << endl;
    int A = 0;
    for (const auto &net : ABnet.cells)
    {
        if (net.size() != 0)
            outFile << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            outFile << "c" << cell << endl;
        }
    }
}

void Partitioning::OutAB(ab now_ABnet)
{
    std::cout << "OutAB cut_size: ";
    std::cout << now_ABnet.cut_size << endl;
    int A = 0;

    // 首先检查 now_ABnet.cells 是否为空
    if (!now_ABnet.cells.empty())
    {
        for (auto &net : now_ABnet.cells)
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
    }
    else
    {
        std::cout << "now_ABnet.cells is empty." << endl;
    }
}

int Partitioning::Cost(ab now_ABnet)
{
    if (now_ABnet.cells.size() >= 2)
        return now_ABnet.cut_size;
    else
        return INT_MAX;
}

ab Partitioning::Perturb(ab &now_ABnet)
{
    ab temp_ABnet;
    temp_ABnet = now_ABnet;
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;

    temp_ABnet.cut.clear();
    for (int i = 1; i <= chooseonecut; i++)
    {
        int r = rand() % (circuit->net_count) + 1;
        temp_ABnet.cut.insert(r);
    }
    temp_ABnet.cut_size = CalCutSize(temp_ABnet);

    // 在新对象上执行 StartPartition
    StartPartition(temp_ABnet);
    cout << "Perturb ok size" << temp_ABnet.cut_size << endl;
    for (auto &i : temp_ABnet.cut)
    {
        cout << "n " << i << endl;
    }
    return temp_ABnet;
}

ab Partitioning::Initial(ab &now_ABnet)
{
    ab temp_ABnet;
    temp_ABnet = now_ABnet;
    temp_ABnet.cut.clear();
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;

    if (temp_ABnet.cut.count(chooseonecut))
        temp_ABnet.cut.erase(chooseonecut); // Mark it as cut
    else
        temp_ABnet.cut.insert(chooseonecut);

    temp_ABnet.cut_size = CalCutSize(temp_ABnet);
    StartPartition(temp_ABnet);
    cout << "Initial ok size" << temp_ABnet.cut_size << endl;
    for (auto &i : temp_ABnet.cut)
    {
        cout << "n " << i << endl;
    }

    return temp_ABnet;
}

void Partitioning::DFS(int current_cell, set<int> &visited, set<int> &current_net, map<int, vector<int>> new_connections)
{
    visited.insert(current_cell);
    current_net.insert(current_cell);

    for (int connected_cell : new_connections[current_cell])
    {
        if (visited.find(connected_cell) == visited.end())
            DFS(connected_cell, visited, current_net, new_connections);
    }
}

void Partitioning::StartPartition(ab &now_ABnet)
{
    map<int, vector<int>> new_connection = DefineConnection(now_ABnet);
    set<int> visited;
    now_ABnet.cells.clear();
    cout << "StartPartion ok" << endl;
    FindConnection(visited, now_ABnet, new_connection);
    cout << "ABMerge ok " << endl;
    ABMerge(now_ABnet);
    OutAB(now_ABnet);
    visited.clear();
}

void Partitioning::FindConnection(set<int> &visited, ab &next_ABnet, map<int, vector<int>> new_connections)
{
    for (const auto &connection : new_connections)
    {
        int current_cell = connection.first;
        if (visited.find(current_cell) == visited.end())
        {
            set<int> current_net;
            DFS(current_cell, visited, current_net, new_connections);
            next_ABnet.cells.push_back(current_net);
        }
    }
}

void Partitioning::ABMerge(ab &next_ABnet)
{
    if (!next_ABnet.cells.empty())
    {
        if (next_ABnet.cells.size() > 2)
        {
            for (int i = 2; i < next_ABnet.cells.size(); i++) // 从索引 2 开始，因为索引 0 和 1 已经被初始化
            {
                if (next_ABnet.cells[0].size() < next_ABnet.cells[1].size())
                {
                    for (int cell : next_ABnet.cells[i])
                        next_ABnet.cells[0].insert(cell);
                }
                else
                {
                    for (int cell : next_ABnet.cells[i])
                        next_ABnet.cells[1].insert(cell);
                }
                next_ABnet.cells[i].clear();
            }
        }
    }

    cout << "ABMerge1 ok" << endl;
    OutAB(next_ABnet);
    // 重新分配剩余的元素到 ABnet 中的两个 set<int> 中
    cout << next_ABnet.cut_size << endl;
    if (next_ABnet.cells.size() > 1)
    {
        for (int i = 1; i <= circuit->cell_count; i++)
        {
            set<int> cell;
            cell.insert(i);
            if (next_ABnet.cells.empty())
                next_ABnet.cells.push_back(cell);
            else if (!FindInABnet(next_ABnet, i))
            {
                if (next_ABnet.cells[0].size() < next_ABnet.cells[1].size())
                    next_ABnet.cells[0].insert(i);
                else
                    next_ABnet.cells[1].insert(i);
                cout << "ABLoc ok" << endl;
            }
            cout << "ABMerge2 process ok" << endl;
            OutAB(next_ABnet);
        }
        cout << "ABMerge2 ok" << endl;
        OutAB(next_ABnet);
    }
}

bool Partitioning::FindInABnet(ab next_ABnet, int cell)
{
    cout << "FindInABnet ok" << endl;
    for (auto &i : next_ABnet.cells)
    {
        if (i.count(cell))
        {
            return true;
        }
    }
    cout << "FindAB false ok" << endl;
    return false;
}

map<int, vector<int>> Partitioning::DefineConnection(ab now_ABnet)
{
    map<int, vector<int>> new_connections;
    new_connections.clear();
    for (const auto &i : circuit->nets) // search all nets
    {
        int j = 0;
        if (now_ABnet.cut.count(i.first))
        {
            while (j < i.second.size()) // search cells in net
            {
                map<int, vector<int>>::iterator iter;
                iter = connections.find(i.second[j]); // find the cell in connections
                if (iter == connections.end())        // don't find the cell in connections
                {
                    vector<int> temp;
                    for (const auto &k : i.second)
                    {
                        if (k != i.second[j]) // i.second[j] is no need to store if it = key value
                        {
                            temp.push_back(k);
                            new_connections[k].push_back(i.second[j]); // to construct double link
                        }
                    }
                    new_connections[i.second[j]] = temp;
                    break;
                }
                j++;
            }
        }
    }
    return new_connections;
}

bool Partitioning::Metropolis(double t, int cost)
{
    double r = static_cast<double>(rand()) / (RAND_MAX + 1.0);
    return (exp(-double(cost) / t) > r);
}

void Partitioning::SimulatedAnnealing()
{
    double ti = 1000, tend = 50;
    double t = ti;

    ab now_ABnet;
    ab next_ABnet;
    now_ABnet = Initial(now_ABnet); // Initialize the partition assignment

    while (t > tend)
    {
        do
        {
            cout << "t ok" << endl;
            next_ABnet = Perturb(now_ABnet);
            cout << "Perturb cut ok" << endl;
            std::cout << "nowAB_cutsize: ok" << next_ABnet.cut_size << endl
                      << endl
                      << endl;
            for (auto &i : next_ABnet.cut)
            {
                cout << "n " << i << endl;
            }

            cout << "Perturb ok now" << endl;
            OutAB(now_ABnet);
            cout << "Perturb ok best" << endl;
            OutAB(ABnet);

            if (Cost(next_ABnet) < Cost(now_ABnet))
            {
                now_ABnet = next_ABnet;
                if (Cost(now_ABnet) < Cost(ABnet) && Constraint1(now_ABnet))
                {
                    ABnet = now_ABnet;
                    cout << "ABrefresh ok" << endl;
                    OutAB(ABnet);
                }
            }
            else if (Metropolis(t, Cost(next_ABnet) - Cost(now_ABnet)))
            {
                now_ABnet = next_ABnet;
            }
        } while ((!Constraint1(ABnet)));
        t = 0.9 * t;
    }
}

bool Partitioning::ABsize(vector<set<int>> now_ABnet)
{
    int total_size = 0;
    if (!now_ABnet.empty()) // 检查ABnet不为空且大小至少为2
    {
        if (now_ABnet.size() == 2)
        {
            for (auto &i : now_ABnet)
            {
                for (auto &j : i)
                    total_size++;
            }
            if (total_size == circuit->cell_count)
            {
                return true;
            }
        }
    }
    return false;
}

bool Partitioning::Constraint1(ab now_ABnet)
{
    if (now_ABnet.cells.size() < 2)
    {
        cout << "Not enough sets in now_ABnet" << endl;
        return false;
    }

    int countA;
    int countB;

    cout << "wrong_constraint1" << endl;
    if (now_ABnet.cells[0].empty() || now_ABnet.cells[1].empty())
        return false;
    else
    {
        countA = now_ABnet.cells[0].size();
        countB = now_ABnet.cells[1].size();
        if (double(abs(countA - countB)) <= double((circuit->cell_count / 5)))
            return true;
        else
            return false;
    }
    return false;
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
