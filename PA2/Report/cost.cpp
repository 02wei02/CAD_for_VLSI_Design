int Partitioning::Cost(AB now_ABnet)
{
    if (now_ABnet->cells.size() >= 2)
        return now_ABnet->cut.size();
    else
        return INT_MAX;
}