#include "graph.h"
#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <utility> // for std::pair

// turn the net connections into graph
void Graph::buildGraph()
{
    string node;
    for (const auto &connection : this->connections)
    {
        node = connection.first;
        const vector<string> &neighbors = connection.second;
        for (const string &neighbor : neighbors)
        {
            auto toB = this->MToB.find(neighbor);
            if (toB != this->MToB.end())
            {
                // cout << "Adding edge: " << node << " -> " << toB->second << endl;
                addEdge(node, toB->second);
            }
            else
                addEdge(node, neighbor);
        }
    }
}

void Graph::Output()
{
    // 输出结果
    cout << "Nodes connected to VSS found by BFS: " << endl;
    for (const string &node : this->result)
    {
        cout << node << " ";
    }
    cout << endl;
    cout << "------------------------------------------------------------------------------\n";
}
void Graph::BFS()
{
    queue<string> q;
    set<string> visited; // represents whether it is visited

    string start = this->npmos.begin()->first; // get the head VSS or VDD
    cout << "V " << start << endl;
    string last = this->npmos.rbegin()->first; // get the end VDD or VSS
    cout << "V " << last << endl;
    q.push(start); // start point
    visited.insert(start);
    this->result.clear();

    while (!q.empty()) // if queue is not empty
    {
        string node = q.front(); // first in first out: the first value in queue
        q.pop();

        this->result.push_back(node);

        const vector<string> &neighbors = this->connections[last]; // if it is close to the end VDD or VSS

        for (const string &neighbor : this->adjacencyList.at(node)) // expand according to the first value
        {
            auto iter = find(neighbors.begin(), neighbors.end(), neighbor);
            if (visited.find(neighbor) == visited.end() && iter == neighbors.end()) // haven't visited and not in end VDD or VSS
            {
                q.push(neighbor);
                visited.insert(neighbor); // mark as visited
            }
        }
    }

    // end neighborhood nodes add in result
    for (const auto &neighbor : this->adjacencyList.at(last))
    {
        this->result.push_back(neighbor);
    }

    // delete the element not satisfy the condition
    this->result.erase(remove_if(this->result.begin(), this->result.end(), [](const string &s)
                                 { return s[0] != 'M' && s[0] != 'm' && s[0] != 'B' && s[0] != 'b'; }),
                       this->result.end());
}

void Graph::SymMapSort()
{
    sort(sym_map.begin(), sym_map.end(), [this](InSym *a, InSym *b)
         { return CompareSymInResult(a, b); });
}

bool Graph::CompareSymInResult(InSym *a, InSym *b)
{
    // Ensure the pointers are not null
    if (a && b)
    {
        // Ensure the vectors in a and b are not empty
        if (!a->sym.empty() && !b->sym.empty())
        {
            auto iter_a = find(result.begin(), result.end(), a->sym[0]);
            auto iter_b = find(result.begin(), result.end(), b->sym[0]);

            if (iter_a != result.end() && iter_b != result.end())
            {
                return iter_a < iter_b;
            }
        }
    }
    return false;
}

void Graph::SymmetryOutput()
{
    cout << ".sym have been sort by result" << endl;
    map<int, set<string>>::iterator iter;

    for (const auto &iter : this->sym_map)
    {
        cout << iter->index << "\t";
        for (const auto &row : iter->sym)
        {
            cout << row << " ";
        }
        cout << endl;
    }

    cout << "-----------------------------------------------------------------------------------\n";
}