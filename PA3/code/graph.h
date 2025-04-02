/* approximate find the adjacency location from VSS(VDD) to VDD(VSS)*/
#ifndef BFS_H
#define BFS_H
//---------------------------------------------------------------------------------------------------
#include "structure.h"
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Graph
{
public:
    // 构造函数接收外部提供的 connections 和 npmos
    Graph(const map<string, vector<string>> &connections,
          const map<string, vector<string>> &npmos,
          const map<string, string> &MToB,
          const vector<InSym *> &sym_map)
    {
        this->connections = connections;
        this->npmos = npmos;
        this->MToB = MToB;
        this->sym_map = sym_map;
    }

    void Run()
    {
        buildGraph();
        BFS();
        Output();
        SymMapSort();
        cout << "result: ";
        for (int i = 0; i < result.size(); i++)
        {
            cout << result[i] << " ";
        }
        cout << endl;
        // SymmetryOutput();
    }
    vector<InSym *> sym_map; // from placement sym_map
    vector<string> result;   // the order close -> far from VSS or VDD

private:
    map<string, vector<string>> adjacencyList; // adjacencyList including net and mos connected by net
    map<string, vector<string>> connections;   // from placement net
    map<string, vector<string>> npmos;         // from placement npmos

    map<string, string> MToB; // M turn into B

    std::map<int, std::vector<std::vector<std::string>>> sorted_map;

    // 添加边
    void addEdge(const string &node1, const string &node2)
    {
        this->adjacencyList[node1].push_back(node2);
        this->adjacencyList[node2].push_back(node1);
    }
    void buildGraph();
    void BFS();
    void Output();
    void SymMapSort();                                                         // start sorting by CompareSymInResult
    bool CompareSymInResult(const vector<string> &a, const vector<string> &b); // rank the sym_map by result
    void SymMapRank();
    void SymmetryOutput();
    bool CompareSymInResult(InSym *a, InSym *b);
};

#endif
//-------------------------------------------------------------------------------------------------------