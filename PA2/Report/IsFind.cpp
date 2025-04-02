bool Partitioning::IsFindABnet(AB next_ABnet, int cell)
{
    // std::cout << "IsFindABnet ok" << endl;
    for (auto &i : next_ABnet->cells)
    {
        if (i.count(cell))
        {
            return true;
        }
    }
    // std::cout << "FindAB false ok" << endl;
    return false;
}