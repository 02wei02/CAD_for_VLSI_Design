void Partitioning::StartPartition(AB now_ABnet)
{
    map<int, set<int>> connection = DefineConnection(now_ABnet);
    set<int> visited;
    FindConnection(visited, now_ABnet, connection);
    ABMerge(now_ABnet);
    visited.clear();
}