int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (argc != 3)
    {
        std::cout << "Can't open!" << endl;
        return 1;
    }

    // If still running, force stop the process
    Partitioning partition(argv[1], argv[2]);
    partition.Run();

    return 0;
}
