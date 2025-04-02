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

using namespace std;

struct ckt
{
    map<int, vector<int>> nets;
    int net_count;
    string name;
    int cell_count;
};

typedef struct ckt *Ckt;

class Partitioning
{
public:
    Partitioning(string input_file, string output_file) : in(std::move(input_file)), out(std::move(output_file)) {}
    void Run()
    {
        InputFile();
        SimulatedAnnealing(ABnet);
        OutABNet();
    }

private:
    void FirstPass(ifstream &);
    void SecondPass(ifstream &);
    void NetCount(int);
    void CellCount(int);
    void SimulatedAnnealing(vector<set<int>> &ABnet);
    int CalculateCost(vector<set<int>>);
    vector<set<int>> Perturb(vector<set<int>>);
    void DefineConnection(map<int, vector<int>> &connections);
    void DFS(int, set<int> &, set<int> &);
    void FindConnection(set<int> &, vector<set<int>> &);
    void OutABNet();
    void StartPartition(vector<set<int>> &next_ABnet);
    bool Constraint1(vector<set<int>> next_ABnet);
    bool Metropolis(double, int);
    void ABMerge(vector<set<int>> &next_ABnet);
    bool FindInABnet(vector<set<int>> &next_ABnet, int i);

    void InputFile();
    void Output();
    void OutAB();

    int CalCutSize();
    bool ABsize(vector<set<int>> now_ABnet);

    Ckt circuit;

    string in;
    string out;
    int net_count = 0;
    int cell_count = 0;

    double ti = 1000, tend = 50;
    map<int, bool> cut; // cut n# = 1

    map<int, vector<int>> connections;
    vector<set<int>> ABnet;
};

void Partitioning::InputFile()
{
    circuit = new ckt;
    ifstream inFile(in);

    circuit->name = in;

    if (!inFile)
    {
        cout << "Input File can't be open!" << endl;
        return;
    }

    FirstPass(inFile);
    inFile.close(); // 關閉文件
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

    cout << right << setw(10) << "NetCount" << setw(10) << "CellCount" << endl
         << setw(10) << circuit->net_count << setw(10) << circuit->cell_count << endl;
    map<int, vector<int>> cell = connections;
    for (const auto &i : cell)
    {
        cout << i.first << setw(5);
        const vector<int> &net = i.second;

        for (const auto &j : net)
        {
            cout << j << " ";
        }
        cout << endl;
    }
}

int Partitioning::CalCutSize()
{
    int sum = 0;
    for (const auto &i : cut)
    {
        if (i.second == 1)
            sum++;
    }
    return sum;
}

void Partitioning::OutABNet()
{
    ofstream outFile(out);

    if (!outFile)
    {
        cout << "Input File can't be open!" << endl;
        return;
    }
    outFile << "cut_size ";
    outFile << CalCutSize() << endl;
    int A = 0;
    for (const auto &net : ABnet)
    {
        outFile << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            outFile << "c" << cell << endl;
        }
    }
}

void Partitioning::OutAB()
{

    cout << "cut_size: ";
    cout << CalCutSize() << endl;
    int A = 0;
    for (const auto &net : ABnet)
    {
        cout << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            cout << "c" << cell << " ";
        }
        cout << endl;
    }
}

int Partitioning::CalculateCost(vector<set<int>> next_ABnet)
{
    int countA;
    int countB;
    countA = next_ABnet[0].size();
    countB = next_ABnet[1].size();

    return abs(countA - countB);
}

vector<set<int>> Partitioning::Perturb(vector<set<int>> now_ABnet)
{
    vector<set<int>> next_ABnet = now_ABnet;
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;
    if (cut[chooseonecut] == true)
        cut[chooseonecut] = false; // Mark it as cut
    else
        cut[chooseonecut] = true;

    StartPartition(next_ABnet);
    return next_ABnet;
}

void Partitioning::DFS(int current_cell, set<int> &visited, set<int> &current_net)
{
    visited.insert(current_cell);
    current_net.insert(current_cell);

    for (int connected_cell : connections[current_cell])
    {
        if (visited.find(connected_cell) == visited.end())
            DFS(connected_cell, visited, current_net);
    }
}

void Partitioning::StartPartition(vector<set<int>> &next_ABnet)
{
    DefineConnection(connections);
    set<int> visited;
    FindConnection(visited, next_ABnet);
    ABMerge(next_ABnet);
    visited.clear();
}

void Partitioning::FindConnection(set<int> &visited, vector<set<int>> &next_ABnet)
{
    next_ABnet.clear();
    for (const auto &connection : connections)
    {
        int current_cell = connection.first;
        if (visited.find(current_cell) == visited.end())
        {
            set<int> current_net;
            DFS(current_cell, visited, current_net);
            next_ABnet.push_back(current_net);
        }
    }
}

void Partitioning::ABMerge(vector<set<int>> &next_ABnet)
{
    if (next_ABnet.size() >= 2)
    {
        for (int i = 2; i < next_ABnet.size(); i++) // 从索引 2 开始，因为索引 0 和 1 已经被初始化
        {
            if (next_ABnet[0].size() < next_ABnet[1].size())
            {
                // 将 next_ABnet[i] 中的元素插入到 ABnet 中的第一个 set<int> 中
                for (int cell : next_ABnet[i])
                {
                    next_ABnet[0].insert(cell);
                }
            }
            else
            {
                // 将 next_ABnet[i] 中的元素插入到 ABnet 中的第二个 set<int> 中
                for (int cell : next_ABnet[i])
                {
                    next_ABnet[1].insert(cell);
                }
            }
        }
    }
    if (next_ABnet.size() > 1)
        // 重新分配剩余的元素到 ABnet 中的两个 set<int> 中
        for (int i = 1; i <= circuit->cell_count; i++)
        {
            if (!FindInABnet(next_ABnet, i))
            {
                if (next_ABnet[0].size() < next_ABnet[1].size())
                {
                    next_ABnet[0].insert(i);
                }
                else
                {
                    next_ABnet[1].insert(i);
                }
            }
        }
}

bool Partitioning::FindInABnet(vector<set<int>> &next_ABnet, int cell)
{
    for (auto &i : next_ABnet)
    {
        for (auto &j : i)
        {
            if (j == cell)
            {
                return true;
                cout << "cell find " << j;
            }
        }
    }
    return false;
}

void Partitioning::DefineConnection(map<int, vector<int>> &connections)
{
    connections.clear();
    for (const auto &i : circuit->nets) // search all nets
    {
        int j = 0;
        if (cut[i.first] != 1)
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
                            connections[k].push_back(i.second[j]); // to construct double link
                        }
                    }
                    connections[i.second[j]] = temp;
                    break;
                }
                j++;
            }
        }
    }
}

bool Partitioning::Metropolis(double t, int cost)
{
    double r = static_cast<double>(rand()) / (RAND_MAX + 1.0);
    return (exp(-double(cost) / t) > r);
}

void Partitioning::SimulatedAnnealing(vector<set<int>> &ABnet)
{
    double t = ti;
    vector<set<int>> now_ABnet;
    vector<set<int>> next_ABnet;

    ABnet = Perturb(ABnet); // Initialize the partition assignment
    OutAB();

    now_ABnet = ABnet;
    next_ABnet = ABnet;

    while (t > tend)
    {
        while (!Constraint1(ABnet))
        {
            next_ABnet = Perturb(now_ABnet);

            if (CalculateCost(next_ABnet) < CalculateCost(now_ABnet))
            {
                now_ABnet = next_ABnet;
                if (CalculateCost(now_ABnet) < CalculateCost(ABnet) && Constraint1(now_ABnet))
                {
                    ABnet = now_ABnet;
                }
            }
            else if (Metropolis(t, CalculateCost(next_ABnet) - CalculateCost(now_ABnet)))
            {
                now_ABnet = next_ABnet;
            }
            Output();
            OutAB();
            cout << "ABnet_size: " << ABnet.size() << endl
                 << endl
                 << endl;
        }
        t = 0.99 * t;
    }
}

bool Partitioning::ABsize(vector<set<int>> now_ABnet)
{
    int total_size = 0;
    if (!now_ABnet.empty()) // 检查ABnet不为空且大小至少为2
    {
        if (now_ABnet.size() == 2)
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
    return false;
}

bool Partitioning::Constraint1(vector<set<int>> now_ABnet)
{
    if (CalculateCost(now_ABnet) <= (circuit->cell_count / 5))
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        cout << "Can't open!" << endl;
        return 1;
    }

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    Partitioning partition(argv[1], argv[2]);
    partition.Run();

    return 0;
}
