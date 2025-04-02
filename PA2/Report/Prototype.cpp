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