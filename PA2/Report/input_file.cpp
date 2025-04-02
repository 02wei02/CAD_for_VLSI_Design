void Partitioning::InputFile()
{
    circuit = new ckt;
    ifstream inFile(in);

    circuit->name = in;

    if (!inFile)
    {
        std::cout << "Input File can't be open!" << endl;
        return;
    }

    FirstPass(inFile);
    inFile.close(); // close file
    std::cout << "Read File" << endl;
}