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