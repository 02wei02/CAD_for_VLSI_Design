// PA1
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;

/* Define the the function name performed by the gate driving this node.*/
enum GATENAME {
    INPT, AND, NAND, OR, NOR, XOR, XNOR, BUFF, NOT, FROM, OUTPT, DRIVER
};

/* Information about a single node in the circuit. */
struct NODE{
    int address;   // A unique number that differentiates the node.
    char* name;    // Provide meaningful information about the node usage. 
    GATENAME type; // The function.
    int numFanin;  // The number of gates. 
    int* fanins;   // Array for the fanin. 
    char* func;
    char *outname;
};

typedef struct NODE *PNODE;

struct CKT{
    PNODE *nodes;     // Array of all node
    char* name;       // Name of the CKT
    int  *inputs;     // Array of input gate.
    int  *outputs;    // Array of output gate.  
    int  numNode;     // The total node to form the CKT.
    int numInput;     // Number of total input wires
    int numOutput;    // Number of total output wires
};

typedef struct CKT *PCKT;


PCKT first_pass(ifstream& inFile, PCKT c);
PCKT second_pass(ifstream& inFile, PCKT c);
GATENAME translateGate(string type);
string print_gate(PCKT c, int i, ofstream &outFile, int j, int);

PCKT first_pass(ifstream& inFile, PCKT c){
    string s;
    int address, fanout, fanin;
    string name, type;
    int numOutput = 0, numInput = 0;
    int maxAddress = -1;
    int lineCount = 0;                  // column line cause by number of fanout

    while(getline(inFile, s)){
        istringstream iss(s);
        if(s[0] == '*') continue;
        iss >> address >> name >> type >> fanout >> fanin;
        lineCount += fanout;
        maxAddress = (address > maxAddress) ? address : maxAddress;
        if(fanout > 0) getline(inFile, s);
        if(fanin > 1){
            while(fanin-- > 0){
                getline(inFile, s);
            }
        }
        if(fanout == 0) numOutput++;
        if(fanin == 0)  numInput++;
    }

    c -> numOutput = numOutput;
    c -> outputs = (int *) malloc (numOutput * sizeof(int));
    c -> numInput = numInput;
    c -> inputs = (int *) malloc (numInput * sizeof(int));
    c -> nodes = (PNODE *) malloc ((maxAddress + lineCount) * sizeof(PNODE));
    cout << "LineCount: " << lineCount << endl;

    return c;
}

PCKT second_pass(ifstream& inFile, PCKT c){

    string s;
    int address, fanout, fanin;
    char name[256], type[256];
    int col = 0;

    while(getline(inFile, s)){
        istringstream iss(s);
        if(s[0] == '*') continue;
        iss >> address >> name >> type >> fanout >> fanin;
        cout << address << " " << name << " " << type << " " << fanout << " " << fanin << endl;


        stringstream add;
            add << address;

            string fullname = "gat" + add.str();

            c->nodes[col] = (NODE *)malloc(sizeof(NODE));
            c->nodes[col]->address = address;
            c->nodes[col]->name = strdup(name);
            c->nodes[col]->type = translateGate(type);
            c->nodes[col]->numFanin = fanin;
            c->nodes[col]->fanins = (int *)malloc(fanin * sizeof(int));
            c->nodes[col]->func = strdup(type);
             c->nodes[col]-> outname = strdup(fullname.c_str());

            cout << c->nodes[col]->address << " " << c->nodes[col]->name << " " << c->nodes[col]->type << " " << c->nodes[col]->numFanin << endl;

            if (fanin > 0)
            {
                getline(inFile, s);
                istringstream iss(s);
                for (int i = 0; i < fanin; i++)
                {
                    iss >> c -> nodes[col] -> fanins[i]; // nodes drive the gate
                    cout << c -> nodes[col] -> fanins[i] << endl; 
            }
            cout << endl;
        }

        cout << "COL: " << col << endl;
        col++;

        if(translateGate(type) != INPT){
            c -> nodes[col] = (NODE *) malloc (sizeof(NODE));
            c -> nodes[col] -> address = address;
            c -> nodes[col] -> name = strdup(name);
            c -> nodes[col] -> type = DRIVER;
            c -> nodes[col] ->numFanin = 1;
            c -> nodes[col] -> fanins = (int *) malloc (fanin * sizeof(int));
            c -> nodes[col] -> func = strdup(type);
            fullname = "gat_out" + add.str();
            c->nodes[col]-> outname = strdup(fullname.c_str());

            if(fanout == 0)
                c -> nodes[col] -> type = OUTPT;

            cout << c -> nodes[col] -> name << endl;
            
            col++;
        }
        
        char fan[256];
        if(fanout > 1)
        {
            for(int i = 0; i < fanout; i++){
                 getline(inFile, s);
                istringstream iss(s);
                iss >> address >> fan;
                c -> nodes[col] = (NODE *) malloc (sizeof(NODE));
                c -> nodes[col] -> address = address;
                c -> nodes[col] -> name = strdup(name);
                c -> nodes[col] -> type = FROM;
                c -> nodes[col] ->numFanin = 1;
                c -> nodes[col] -> fanins = (int *) malloc (sizeof(int));
                c -> nodes[col] -> fanins[0] = address;
                c -> nodes[col] -> func = strdup(type);
                c->nodes[col]-> outname = strdup(fullname.c_str());
                if(translateGate(type) != INPT)
                {
                    fullname = "gat_out" + add.str();
                    c->nodes[col]-> outname = strdup(fullname.c_str());
                }

                cout << c -> nodes[col] -> name << endl;
                col++;
            }
        }

        c-> numNode = col;
        
    }
    return c;
}

GATENAME translateGate(string type){
    if(type == "inpt")
        return INPT;
    if(type == "and")
        return AND;
    if(type == "nand")
        return NAND;
    if(type == "or")
        return OR;
    if(type == "nor")
        return NOR;
    if(type == "xor")
        return XOR;
    if(type == "xnor")
        return XNOR;
    if(type == "buff")
        return BUFF;
    if(type == "not")
        return NOT;
    if(type == "from")
        return FROM;
    return DRIVER;
}


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

void output_file(char *vFile, PCKT c){

    ofstream outFile(vFile);

    if(!outFile){
        cout << "v can't be generate.";
    }
    
    outFile << "`timescale 1ns/1ps\n";

    bool notFirst = 0;
    // module 
    outFile << "module " << string(c->name).substr(0, string(c->name).length() - 4) << " (";
    for(int i = 0; i < c-> numNode; i++){
        if(c -> nodes[i] -> type == INPT || c -> nodes[i] -> type == OUTPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ");\n";

    
    // input
    notFirst = 0;
    outFile << "input ";
    for(int i = 0; i < c->numNode; i++){
        if(c -> nodes[i] -> type == INPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ";\n";

    // outputs
    notFirst = 0;
    outFile << "output ";
    for(int i = 0; i < c->numNode; i++){
        if(c -> nodes[i] -> type == OUTPT){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ";\n";

    // wire
    notFirst = 0;
    outFile << "wire ";
    notFirst = 0;
    for(int i = 0; i < c -> numNode; i++){

        if(c -> nodes[i] -> type == DRIVER){
            if(notFirst)
                outFile << ", ";
            outFile << c -> nodes[i] -> outname;
            notFirst = 1;
        }
    }
    outFile << ";\n\n";

    for(int i = 0; i < c -> numNode; i++){
        if(c -> nodes[i] -> type != INPT 
        && c -> nodes[i] -> type != OUTPT 
        && c -> nodes[i] -> type != DRIVER
        && c -> nodes[i] -> type != FROM){
            outFile << c -> nodes[i] -> func << " ";
            outFile << c -> nodes[i] -> outname << " ( "; 
            outFile << c -> nodes[i+1] -> outname ;
            for(int j = 0; j < c -> nodes[i] -> numFanin; j++)
            {
                for (int k = 0; k < c->numNode; k++) {
                    outFile << print_gate (c, i, outFile, j, k);
                }
            }
            outFile << " );\n";
        }
    }

    outFile << "\nendmodule\n";
    
}

string print_gate(PCKT c, int i, ofstream &outFile, int j, int k) {
    string result;  // String to hold the concatenated names
    
    if (c->nodes[k]->address == c->nodes[i]->fanins[j]) {
        if(c -> nodes[k] -> type == INPT)
            result += ", " + string(c->nodes[k]-> outname);
        else if (c->nodes[k]->type == FROM)
            result += ", " + string(c->nodes[k]-> outname);
        else if(c->nodes[k]->type == DRIVER)
            result += ", " + string(c->nodes[k]-> outname);

        return result;
    }
    // If the condition is not met, return an empty string
    return "";
}


int main(int argc, char *argv[]){
    PCKT c;
    
    c = input_file(argv[1]);
    
    cout << "FIND " << c -> nodes[5] -> name << endl;
    output_file(argv[2], c);

    free(c);

    return 0;

}