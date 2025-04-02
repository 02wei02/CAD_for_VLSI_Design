map<int, set<int>> Partitioning::DefineConnection(AB now_ABnet)
{
    map<int, set<int>> next_connections;
    next_connections.clear();
    for (const auto &i : circuit->nets) // search all nets
    {
        int j = 0;
        if (!now_ABnet->cut.count(i.first))
        {
            for (auto &j : i.second)
            {
                set<int> temp;
                for (const auto &k : i.second)
                {
                    next_connections[j].insert(k);
                    next_connections[k].insert(j);
                }
            }
        }
    }
    return next_connections;
}