PCKT input_file(char *iscFile){

    PCKT c;
    ifstream inFile(iscFile);

    c = (PCKT) malloc (sizeof(CKT));

    if(!inFile){
        cout << "isc file can't not be open.";
    }

    c->name = iscFile;

    c = first_pass(inFile, c);

    
    inFile.clear(); // Clear any error flags
    inFile.seekg(0, ios::beg); // Reset file pointer to beginning

    c = second_pass(inFile, c);

    cout << "NO " << c -> nodes[0] -> name << endl;

    inFile.close(); // Close the file

    return c;
}