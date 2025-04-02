void Partitioning::OutABNet()
{
    ofstream outFile(out);

    if (!outFile)
    {
        std::cout << "Input File can't be open!" << endl;
        return;
    }
    outFile << "cut_size ";
    outFile << ABnet->cut_size << endl;
    int A = 0;
    for (const auto &net : ABnet->cells)
    {
        if (net.size() != 0)
            outFile << char('A' + A) << endl;
        A++;
        for (int cell : net)
        {
            outFile << "c" << cell << endl;
        }
    }
    delete (ABnet);
}