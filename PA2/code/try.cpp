#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

// 定义一个结构表示每个连线（网）上的单元格
struct Net
{
    set<int> cells; // 连线上的单元格集合
};

// 使用 DFS 算法遍历连线图，确定哪些单元格在同一条连线上
void DFS(int currentCell, map<int, vector<int>> &connections, set<int> &visited, Net &currentNet)
{
    visited.insert(currentCell);
    currentNet.cells.insert(currentCell);

    for (int connectedCell : connections[currentCell])
    {
        if (visited.find(connectedCell) == visited.end())
        {
            DFS(connectedCell, connections, visited, currentNet);
        }
    }
}

// 主函数用于检查哪些单元格在同一条连线上
int main()
{

    // 记录已访问的单元格
    set<int> visited;

    // 存储所有连线的集合
    vector<Net> nets;

    // 定义连接关系
    map<int, vector<int>> connections;
    connections[1] = {3, 5, 7}; // c1 与 c3 相连
    connections[2] = {3, 4};    // c2 与 c4、c6 相连
    connections[3] = {7, 5, 1}; // c3 与 c1、c4 相连
    connections[4] = {2, 6};    // c4 与 c2、c3、c5 相连
    connections[5] = {3, 7, 1}; // c5 与 c4 相连
    connections[6] = {4};       // c6 与 c2、c7 相连
    connections[7] = {3, 5, 1};
    connections[8] = {2}; // c7 与 c6 相连

    for (const auto &connection : connections)
    {
        int currentCell = connection.first;
        if (visited.find(currentCell) == visited.end())
        {
            // 如果当前单元格未访问，则开始一条新的连线
            Net currentNet;
            DFS(currentCell, connections, visited, currentNet);
            nets.push_back(currentNet);
        }
    }

    // 输出结果
    for (const auto &net : nets)
    {
        cout << "{ ";
        for (int cell : net.cells)
        {
            cout << "c" << cell << " ";
        }
        cout << "}" << endl;
    }

    connections.clear();
    nets.clear();
    visited.clear();
    cout << endl
         << endl;

    connections[1] = {3, 5, 7}; // c1 与 c3 相连
    connections[2] = {4, 8};    // c2 与 c4、c6 相连
    connections[3] = {7, 5, 1}; // c3 与 c1、c4 相连
    connections[4] = {2, 6};    // c4 与 c2、c3、c5 相连
    connections[5] = {3, 7, 1}; // c5 与 c4 相连
    connections[6] = {4};       // c6 与 c2、c7 相连
    connections[7] = {3, 5, 1};
    connections[8] = {2}; // c7 与 c6 相连

    // 遍历每个单元格
    for (const auto &connection : connections)
    {
        int currentCell = connection.first;
        if (visited.find(currentCell) == visited.end())
        {
            // 如果当前单元格未访问，则开始一条新的连线
            Net currentNet;
            DFS(currentCell, connections, visited, currentNet);
            nets.push_back(currentNet);
        }
    }

    // 输出结果
    for (const auto &net : nets)
    {
        cout << "{ ";
        for (int cell : net.cells)
        {
            cout << "c" << cell << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}
