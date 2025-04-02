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

struct ab
{
    vector<set<int>> cells;
    map<int, bool> cut; // cut n# = 1
    int cut_size;
};

typedef struct ab *AB;

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
    int Cost(AB);
    AB Perturb(AB &);
    AB Initial(AB &);
    map<int, vector<int>> DefineConnection(AB);
    void DFS(int, set<int> &, set<int> &, map<int, vector<int>>);
    void FindConnection(set<int> &, vector<set<int>> &, map<int, vector<int>>);
    void OutABNet();
    void StartPartition(AB &next_ABnet);
    bool Constraint1(vector<set<int>> next_ABnet);
    bool Metropolis(double, int);
    void ABMerge(vector<set<int>> &next_ABnet);
    bool FindInABnet(vector<set<int>> &next_ABnet, int i);

    void InputFile();
    void Output();
    void OutAB(AB);

    int CalCutSize(AB);
    bool ABsize(vector<set<int>> now_ABnet);

    Ckt circuit = new ckt;

    string in;
    string out;
    int net_count = 0;
    int cell_count = 0;

    map<int, vector<int>> connections;
    AB ABnet = new ab;
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

int Partitioning::CalCutSize(AB now_ABnet)
{
    int sum = 0;
    for (const auto &i : now_ABnet->cut)
    {
        if (i.second == true)
            sum++;
    }
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
    outFile << CalCutSize(ABnet) << endl;
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
}

void Partitioning::OutAB(AB now_ABnet)
{

    std::cout << "cut_size: ";
    std::cout << CalCutSize(now_ABnet) << endl;
    int A = 0;
    for (const auto &net : now_ABnet->cells)
    {
        if (net.size() > 0)
            std::cout << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            std::cout << "c" << cell << " ";
        }
        std::cout << endl;
    }
}

int Partitioning::Cost(AB now_ABnet)
{
    return now_ABnet->cut_size;
}

AB Partitioning::Perturb(AB &now_ABnet)
{
    AB next_ABnet = now_ABnet;
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;
    next_ABnet->cut.clear();
    for (int i = 1; i <= chooseonecut; i++)
    {
        int r = rand() % (circuit->net_count) + 1;
        next_ABnet->cut[r] = true;
    }
    StartPartition(next_ABnet);
    return next_ABnet;
}

AB Partitioning::Initial(AB &now_ABnet)
{
    AB next_ABnet = now_ABnet;
    // Randomly choose a cell to cut
    int chooseonecut = rand() % (circuit->net_count) + 1;
    next_ABnet->cut.clear();
    if (next_ABnet->cut[chooseonecut] == true)
        next_ABnet->cut[chooseonecut] = false; // Mark it as cut
    else
        next_ABnet->cut[chooseonecut] = true;

    StartPartition(next_ABnet);
    return next_ABnet;
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

void Partitioning::StartPartition(AB &next_ABnet)
{
    next_ABnet->cells.clear();
    map<int, vector<int>> new_connection = DefineConnection(next_ABnet);
    set<int> visited;
    FindConnection(visited, next_ABnet->cells, new_connection);
    ABMerge(next_ABnet->cells);
    next_ABnet->cut_size = CalCutSize(next_ABnet);
    visited.clear();
}

void Partitioning::FindConnection(set<int> &visited, vector<set<int>> &next_ABnet, map<int, vector<int>> new_connections)
{
    for (const auto &connection : new_connections)
    {
        int current_cell = connection.first;
        if (visited.find(current_cell) == visited.end())
        {
            set<int> current_net;
            DFS(current_cell, visited, current_net, new_connections);
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
                next_ABnet[0].insert(next_ABnet[i].begin(), next_ABnet[i].end());
            }
            else
            {
                // 将 next_ABnet[i] 中的元素插入到 ABnet 中的第二个 set<int> 中
                next_ABnet[1].insert(next_ABnet[i].begin(), next_ABnet[i].end());
            }
            // 清空 next_ABnet[i]
            next_ABnet[i].clear();
        }
    }

    // 重新分配剩余的元素到 ABnet 中的两个 set<int> 中
    for (int i = 1; i <= circuit->cell_count; i++)
    {
        if (!FindInABnet(next_ABnet, i))
        {
            // 找到当前 cell 对应的 set<int>
            set<int> &chosen_set = (next_ABnet[0].size() < next_ABnet[1].size()) ? next_ABnet[0] : next_ABnet[1];
            chosen_set.insert(i);
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
            }
        }
    }
    return false;
}

map<int, vector<int>> Partitioning::DefineConnection(AB now_ABnet)
{
    map<int, vector<int>> new_connections;
    for (const auto &i : circuit->nets) // search all nets
    {
        int j = 0;
        if (now_ABnet->cut[i.first] == false)
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
    AB now_ABnet = new ab;
    AB next_ABnet = new ab;

    ABnet = Initial(ABnet); // Initialize the partition assignment
    OutAB(ABnet);

    now_ABnet = ABnet;
    next_ABnet = ABnet;

    while (t > tend)
    {
        do
        {
            next_ABnet = Perturb(now_ABnet);

            if (Cost(next_ABnet) < Cost(now_ABnet))
            {
                now_ABnet = next_ABnet;
                if (Cost(now_ABnet) < Cost(ABnet) && Constraint1(now_ABnet->cells))
                {
                    ABnet = now_ABnet;
                }
            }
            else if (Metropolis(t, Cost(next_ABnet) - Cost(now_ABnet)))
            {
                now_ABnet = next_ABnet;
            }
            Output();
            OutAB(now_ABnet);
            std::cout << "ABnet_size: " << ABnet->cells.size() << endl
                      << endl
                      << endl;
        } while ((!Constraint1(ABnet->cells)));
        t = 0.99 * t;
    }

    cout << "cut: ";
    for (auto &i : ABnet->cut)
    {
        if (i.second == true)
        {
            cout << i.first << " ";
        }
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

bool Partitioning::Constraint1(vector<set<int>> now_ABnet)
{
    int countA;
    int countB;

    if (now_ABnet[0].empty() || now_ABnet[1].empty())
        return 0;
    else
    {
        countA = now_ABnet[0].size();
        countB = now_ABnet[1].size();
        if (abs(countA - countB) <= (circuit->cell_count / 5))
            return 1;
        else
            return 0;
    }
    return 0;
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
