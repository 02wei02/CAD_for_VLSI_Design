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
        SimulatedAnnealing();
        OutABNet();
    }

private:
    void FirstPass(ifstream &);
    void SecondPass(ifstream &);
    void NetCount(int);
    void CellCount(int);
    void SimulatedAnnealing();
    int CalculateCost(vector<set<int>>);
    vector<set<int>> Perturb(vector<set<int>>);
    void DefineConnection();
    void DFS(int, set<int> &, set<int> &);
    void FindConnection(set<int> &, vector<set<int>> &);
    void OutABNet();
    void StartPartition(vector<set<int>> &next_ABnet);
    bool Constraint1(vector<set<int>> next_ABnet);
    bool Metropolis(double, int);

    void InputFile();
    void Output();
    void OutputFile();

    int CalCutSize();

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