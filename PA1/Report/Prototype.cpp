using namespace std;

/* Define the the function name performed by the gate driving this node.*/
enum GATENAME {
    INPT, FROM, OUTPT, DRIVER
};

/* Information about a single node in the circuit. */
struct NODE{
    int address;   // A unique number that differentiates the node.
    char* name;    // Provide meaningful information about the node usage. 
    GATENAME type; // The function.
    int numFanin;  // The number of gates. 
    int* fanins;   // Array for the fanin. 
    char* func;    // Name of the function
    char *outname;  // Port name to ofstream into the .v file.
};

typedef struct NODE *PNODE;

struct CKT{
    PNODE *nodes;     // Array of all node
    char* name;       // Name of the CKT
    int  numNode;     // The total node to form the CKT.
    int numInput;     // Number of total input wires
    int numOutput;    // Number of total output wires
    int numDriver;    // Number of total driver wires
};

typedef struct CKT *PCKT;


PCKT first_pass(ifstream& inFile, PCKT c);
PCKT second_pass(ifstream& inFile, PCKT c);
PCKT input_file(char *iscFile);
void output_file(char *vFile, PCKT c);
